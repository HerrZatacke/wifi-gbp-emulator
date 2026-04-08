import './index.scss';
import { $fetch } from 'ofetch';
import { URL_GET_WIFICONFIG, URL_SET_WIFICONFIG } from '../../consts.ts';
import { type WiFiConfigResponse, WifiConfigUpdateRequest } from '../../types/api.ts';
import { genericButton } from '../generic/button';
import { GenericInput, genericInput } from '../generic/input';

interface RefreshParams {
  mdnsInput: GenericInput;
  ssidInput: GenericInput;
  pskInput: GenericInput;
}

const refreshForm = async ({
  mdnsInput,
  ssidInput,
  pskInput,
}: RefreshParams) => {
  const wifiConfig = await $fetch<WiFiConfigResponse>(URL_GET_WIFICONFIG);

  mdnsInput.input.value = wifiConfig.mdns || '';
  ssidInput.input.value = wifiConfig.ap.ssid || '';
  pskInput.input.value = '';
};

const renderModule = async (moduleRoot: HTMLElement) => {

  const mdnsInput = genericInput({
    title: 'mDNS Name (Bonjour/Avahi)',
    initialValue: '',
  });

  const ssidInput = genericInput({
    title: 'Accesspoint SSID',
    initialValue: '',
  });

  const pskInput = genericInput({
    title: 'Accesspoint Password',
    type: 'password',
    initialValue: '',
  });

  await refreshForm({ mdnsInput, ssidInput, pskInput });

  const saveHostButton = genericButton({
    onClick: async () => {
      const hostUdpate: WifiConfigUpdateRequest = {
        mdns: mdnsInput.input.value,
      };

      if (pskInput.input.value && ssidInput.input.value) {
        hostUdpate.ap = {
          ssid: ssidInput.input.value,
          psk: pskInput.input.value,
        };
      }

      const updateResult = await $fetch<string>(URL_SET_WIFICONFIG, {
        method: 'POST',
        body: JSON.stringify(hostUdpate),
      });

      console.log({ updateResult });

      await refreshForm({ mdnsInput, ssidInput, pskInput });
    },
    className: 'wifi-config__save-button',
    title: 'Save',
  });

  const hostSettings = document.createElement('div');
  hostSettings.className = 'wifi-config__fields';

  const hedaline = document.createElement('h2');
  hedaline.innerText = 'Host Settings';

  hostSettings.appendChild(hedaline);
  hostSettings.appendChild(mdnsInput.node);
  hostSettings.appendChild(ssidInput.node);
  hostSettings.appendChild(pskInput.node);
  hostSettings.appendChild(saveHostButton);

  moduleRoot.appendChild(hostSettings);
};

export const wifiConfig = (): HTMLElement => {
  const moduleRoot = document.createElement('div');
  renderModule(moduleRoot);
  return moduleRoot;
};
