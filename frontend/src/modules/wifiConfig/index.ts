import { $fetch } from 'ofetch';
import { URL_GET_WIFICONFIG } from '../../consts.ts';


const renderModule = async (moduleRoot: HTMLElement) => {
  const wifiConfig = await $fetch(URL_GET_WIFICONFIG);
  moduleRoot.innerHTML = JSON.stringify(wifiConfig, null, 2);
};

export const wifiConfig = (): HTMLElement => {
  const moduleRoot = document.createElement('pre');
  renderModule(moduleRoot);
  return moduleRoot;
};
