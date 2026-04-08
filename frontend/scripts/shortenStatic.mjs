import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import { globby } from 'globby';
import copyAndGZ from './copyAndGZ.mjs';

const inBaseDir = 'out';
const outBaseDir = 'o';
const outFilesDir = path.posix.join(outBaseDir, 'w');

const filesToShorten = await globby([
  `${inBaseDir}/**/*.*`,
  `!${inBaseDir}/dumps/**`,
]);

const getShortName = async (filePath, ext) => {
  const fileContents = await fs.promises.readFile(filePath);
  const hash = crypto.createHash('sha256')
    .update(ext)
    .update(fileContents)
    .digest('hex');
  return hash.slice(0, 6) + ext;
};

// Rename files and store mapping
const mapFileContent = [];
for (const filePath of filesToShorten) {
  const ext = path.posix.extname(filePath);
  const newName = await getShortName(filePath, ext);
  const newPath = path.posix.join(outFilesDir, newName);

  await fs.promises.mkdir(path.posix.dirname(newPath), { recursive: true });
  await copyAndGZ(filePath, newPath);

  const newRelPath = path.posix.relative(outFilesDir, newPath);
  const originalRelPath = path.posix.relative(inBaseDir, filePath);

  mapFileContent.push(`${originalRelPath};${newRelPath}`);
}

mapFileContent.sort((a, b) => (
  a.localeCompare(b)
));

await fs.promises.writeFile(path.posix.join(outBaseDir, 'pathmap.txt'), mapFileContent.join('\n'), { encoding: 'utf-8' });
