import 'reset-css/reset.css';
import './styles/style.scss';
import { $fetch } from 'ofetch';
import {
  URL_ENV_INFO,
  URL_GET_WIFICONFIG,
  URL_LIST_DUMPS,
} from './consts.ts';

(async () => {
  const footer = document.querySelector('.footer') as HTMLElement;
  const main = document.querySelector('.main') as HTMLElement;


  const info = await $fetch(URL_ENV_INFO);
  footer.innerHTML = `<pre>${JSON.stringify(info, null, 2)}</pre>`;


  const dumpsList = await $fetch(URL_LIST_DUMPS);
  const wifiConfig = await $fetch(URL_GET_WIFICONFIG);
  main.innerHTML = `<pre>${JSON.stringify({ dumpsList, wifiConfig }, null, 2)}</pre>`;
})();
