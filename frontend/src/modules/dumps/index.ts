import './index.scss';
import { saveAs } from 'file-saver';
import { $fetch } from 'ofetch';
import { URL_CLEAR_DUMPS, URL_LIST_DUMPS } from '../../consts.ts';
import { type ClearDumpsResponse, type ListDumpsResponse } from '../../types/api.ts';
import { genericButton } from '../generic/button';

const refreshButton = genericButton({
  onClick: async () => {
    await refreshInfo();
  },
  title: 'Refresh Info',
  className: 'dumps__button',
});

const clearButton = genericButton({
  onClick: clearDumps,
  title: 'Clear Images',
  className: 'dumps__button',
});

const downloadButton = genericButton({
  onClick: downloadDumps,
  title: '',
  className: 'dumps__button',
});

const progress = document.createElement('progress');
progress.className = 'dumps__progress';
progress.value = 0;

const disableButtons = () => {
  refreshButton.disabled = true;
  clearButton.disabled = true;
  downloadButton.disabled = true;
};

async function refreshInfo(): Promise<ListDumpsResponse> {
  disableButtons();
  const dumpsInfo = await $fetch<ListDumpsResponse>(URL_LIST_DUMPS);
  const hasDumps = Boolean(dumpsInfo.fs.dumpcount);
  refreshButton.disabled = false;
  clearButton.disabled = !hasDumps;
  downloadButton.disabled = !hasDumps;
  downloadButton.innerHTML = hasDumps ? `Download ${dumpsInfo.fs.dumpcount} Images` : 'No Images';

  return dumpsInfo;
}

async function clearDumps(): Promise<void> {
  const clearDumpsResponse = await $fetch<ClearDumpsResponse>(URL_CLEAR_DUMPS);
  if (clearDumpsResponse.deleted) {
    await refreshInfo();
  }
}

async function downloadDumps(): Promise<void> {
  const { dumps } = await refreshInfo();
  if (!dumps.length) {
    return;
  }

  disableButtons();

  const output: string[] = [];
  const outFilename = `dumps_${new Date().toISOString().slice(0, 16).replace('T', '_').replace(':', '-')}.txt`;

  progress.max = dumps.length;
  progress.value = 0;

  for (const dumpUrl of dumps) {
    const response = await $fetch(dumpUrl, { responseType: 'arrayBuffer' });

    const header = new TextDecoder().decode(response.slice(0, 8));
    if (header === 'GB-BIN01') {
      output.push('!{"command":"INIT"}');
      output.push('!{"command":"DATA","compressed":0,"more":1}');
      const bytes = new Uint8Array(response.slice(8));

      const bytesPerLine = 16;
      for (let index = 0; index < bytes.length; index += bytesPerLine) {
        const lineBytes = bytes.slice(index, index + bytesPerLine);
        const line = Array.from(lineBytes)
          .map(byte => (
            byte.toString(16).toUpperCase().padStart(2, '0')
          ))
          .join(' ');

        output.push(line);
      }

      output.push('!{"command":"DATA","compressed":0,"more":0}');
      output.push('!{"command":"PRNT","sheets":1,"margin_upper":0,"margin_lower":3,"pallet":228,"density":64 }');
    }

    progress.value += 1;
  }

  await saveAs(new File([output.join('\n')], outFilename, { type: 'text/plain' }), outFilename);
  await refreshInfo();

  progress.value = 0;
}

const renderModule = async (moduleRoot: HTMLElement) => {
  await refreshInfo();
  const buttonGroup = document.createElement('div');
  buttonGroup.className = 'dumps__buttons';
  buttonGroup.appendChild(downloadButton);
  buttonGroup.appendChild(clearButton);
  buttonGroup.appendChild(refreshButton);
  moduleRoot.appendChild(buttonGroup);
  moduleRoot.appendChild(progress);
};

export const dumps = (): HTMLElement => {
  const moduleRoot = document.createElement('div');
  renderModule(moduleRoot);
  return moduleRoot;
};
