import { createReadStream, createWriteStream } from 'node:fs';
import { pipeline } from 'node:stream';
import { createGzip } from 'node:zlib';

const copyAndGZ = async (source, destination) => {
  return new Promise((resolve) => {
    pipeline(
      createReadStream(source),
      createGzip(),
      createWriteStream(`${destination}.gz`),
      resolve,
    );
  });
};

export default copyAndGZ;
