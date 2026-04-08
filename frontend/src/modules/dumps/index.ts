import { $fetch } from 'ofetch';
import { URL_LIST_DUMPS } from '../../consts.ts';


const renderModule = async (moduleRoot: HTMLElement) => {
  const dumpsList = await $fetch(URL_LIST_DUMPS);
  moduleRoot.innerHTML = JSON.stringify(dumpsList, null, 2);
};

export const dumps = (): HTMLElement => {
  const moduleRoot = document.createElement('pre');
  renderModule(moduleRoot);
  return moduleRoot;
};
