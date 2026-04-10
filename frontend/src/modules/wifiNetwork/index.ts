import './index.scss';
import {
  type NetworkDeleteRequest,
  type NetworkResponse,
  type NetworkUpdateRequest,
  type UpdateFn,
} from '../../types/api.ts';
import { genericButton } from '../generic/button';
import { genericInput } from '../generic/input';

const renderModule = async (moduleRoot: HTMLElement, updateFn: UpdateFn, config?: NetworkResponse) => {
  const isNewNetwork = !config;

  const ssidInput = genericInput({
    title: 'Accesspoint SSID',
    readonly: !isNewNetwork,
    initialValue: config?.ssid || '',
  });

  const pskInput = genericInput({
    title: 'Accesspoint Password',
    type: 'password',
    initialValue: '',
  });

  const buttons = document.createElement('div');
  buttons.className = 'wifi-network__buttons';

  const saveNetworkButton = genericButton({
    onClick: async () => {
      const networkUdpateRequest: NetworkUpdateRequest = {
        ssid: ssidInput.input.value,
        psk: pskInput.input.value,
      };
      updateFn(networkUdpateRequest);
    },
    className: 'wifi-network__save-button',
    title: isNewNetwork ? 'Add' : 'Save',
  });

  const deleteNetworkButton = genericButton({
    onClick: async () => {
      const networkDeleteRequest: NetworkDeleteRequest = {
        ssid: ssidInput.input.value,
        delete: true,
      };
      updateFn(networkDeleteRequest);
    },
    className: 'wifi-network__delete-button',
    title: 'Delete',

  });

  if (!isNewNetwork) {
    buttons.appendChild(deleteNetworkButton);
  }
  buttons.appendChild(saveNetworkButton);

  const networkSettings = document.createElement('div');
  networkSettings.className = 'wifi-network__fields';

  networkSettings.appendChild(ssidInput.node);
  networkSettings.appendChild(pskInput.node);
  networkSettings.appendChild(buttons);

  moduleRoot.appendChild(networkSettings);
};

export const wifiNetwork = (updateFn: UpdateFn, config?: NetworkResponse): HTMLElement => {
  const moduleRoot = document.createElement('div');
  renderModule(moduleRoot, updateFn, config);
  return moduleRoot;
};
