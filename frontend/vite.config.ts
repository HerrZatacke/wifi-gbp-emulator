import * as fs from 'fs';
import * as path from 'path';
import { $fetch } from 'ofetch';
import { defineConfig, loadEnv } from 'vite';

const input = fs.readdirSync(__dirname)
  .reduce((acc: string[], file) => {
    const filePath = path.join(__dirname, file);
    const stats = fs.statSync(filePath);

    if (!stats.isDirectory() && path.extname(file) === '.html') {
      return [...acc, path.resolve(filePath)];
    }

    return acc;
  }, []);

export default defineConfig(({ mode }) => {
  const env = loadEnv(mode, process.cwd());
  const targetUrl = env.VITE_DEV_PROXY_TARGET || 'http://192.168.4.1';

  return {
    build: {
      outDir: '../fs-out/',
      emptyOutDir: true,
      rolldownOptions: {
        input,
      },
    },
    plugins: [
      {
        name: 'custom-fetch-proxy',
        configureServer: async (server) => {
          console.log(`Starting proxy to ${targetUrl}`);
          let initialResponse: object | null = null;
          try {
            initialResponse = await $fetch(`${targetUrl}/env.json`, { timeout: 1000 });
          } catch { /**/
          }

          if (!initialResponse) {
            console.warn(`Device at ${targetUrl} is not reachable`);
          } else {
            console.error(`Device at ${targetUrl} reports`, initialResponse);
          }

          server.middlewares.use(async (req, res, next) => {
            if (
              req.url?.startsWith('/dumps/') ||
              req.url?.startsWith('/wificonfig/') ||
              req.url?.startsWith('/env.json')
            ) {
              try {
                const url = `${targetUrl}${req.url}`;
                const targetResponse = await $fetch.raw(url, {
                  method: 'GET',
                  responseType: 'arrayBuffer',
                  ignoreResponseError: true,
                });
                res.statusCode = targetResponse.status;
                targetResponse.headers.forEach((value, name) => res.setHeader(name, value));
                res.end(new Uint8Array(targetResponse._data || []));
              } catch (error) {
                console.error('Fetch proxy error:', error);
                res.statusCode = 500;
                res.end('Internal Server Error');
              }
            } else {
              next();
            }
          });
        },
      },
    ],
    server: {
      port: 3000,
    },
  };
});
