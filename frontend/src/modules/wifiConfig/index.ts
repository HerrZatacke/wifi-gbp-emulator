import './index.scss';
import { $fetch } from 'ofetch';
import { URL_GET_WIFICONFIG, URL_SET_WIFICONFIG } from '../../consts.ts';
import {type UpdateFn, type WiFiConfigResponse, type WifiConfigUpdateRequest} from '../../types/api.ts';
import { genericButton } from '../generic/button';
import { GenericInput, genericInput } from '../generic/input';
import { wifiNetwork } from '../wifiNetwork';

interface RefreshParams {
  mdnsInput: GenericInput;
  ssidInput: GenericInput;
  pskInput: GenericInput;
  networks: HTMLDivElement;
  updateFn: UpdateFn,
}

const refreshForm = async ({
  mdnsInput,
  ssidInput,
  pskInput,
  networks,
  updateFn,
}: RefreshParams) => {
  const wifiConfig = await $fetch<WiFiConfigResponse>(URL_GET_WIFICONFIG);

  mdnsInput.input.value = wifiConfig.mdns || '';
  ssidInput.input.value = wifiConfig.ap.ssid || '';
  pskInput.input.value = '';

  networks.innerText = '';

  const headline = document.createElement('h2');
  headline.innerText = 'Network Settings';
  networks.appendChild(headline);

  for (const networkConfig of wifiConfig.networks) {
    networks.appendChild(wifiNetwork(updateFn, networkConfig));
  }
  networks.appendChild(wifiNetwork(updateFn));
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

  const networks = document.createElement('div');
  networks.className = 'wifi-config__networks';

  const updateFn: UpdateFn = async (request) => {
    const hostUdpate: WifiConfigUpdateRequest = {
      networks: [request],
    };

    const updateResult = await $fetch<string>(URL_SET_WIFICONFIG, {
      method: 'POST',
      body: JSON.stringify(hostUdpate),
    });

    console.log({ updateResult });

    await refreshForm({ mdnsInput, ssidInput, pskInput, networks, updateFn });
  };

  await refreshForm({ mdnsInput, ssidInput, pskInput, networks, updateFn });

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

      await refreshForm({ mdnsInput, ssidInput, pskInput, networks, updateFn });
    },
    className: 'wifi-config__save-button',
    title: 'Save',
  });

  const hostSettings = document.createElement('div');
  hostSettings.className = 'wifi-config__fields';

  const headline = document.createElement('h2');
  headline.innerText = 'Host Settings';

  hostSettings.appendChild(headline);
  hostSettings.appendChild(mdnsInput.node);
  hostSettings.appendChild(ssidInput.node);
  hostSettings.appendChild(pskInput.node);
  hostSettings.appendChild(saveHostButton);

  moduleRoot.appendChild(hostSettings);
  moduleRoot.appendChild(networks);
};

export const wifiConfig = (): HTMLElement => {
  const moduleRoot = document.createElement('div');
  moduleRoot.className = 'wifi-config';
  renderModule(moduleRoot);
  return moduleRoot;
};
