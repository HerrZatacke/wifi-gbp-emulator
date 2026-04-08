import { $fetch } from 'ofetch';
import { URL_ENV_INFO } from '../../consts.ts';


const renderModule = async (moduleRoot: HTMLElement) => {
  const info = await $fetch(URL_ENV_INFO);
  moduleRoot.innerHTML = JSON.stringify(info, null, 2);
};

export const footer = (): HTMLElement => {
  const moduleRoot = document.createElement('pre');
  renderModule(moduleRoot);
  return moduleRoot;
};
