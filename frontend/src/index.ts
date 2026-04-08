import 'reset-css/reset.css';
import './styles/style.scss';
import { dumps } from './modules/dumps';
import { footer } from './modules/footer';
import { wifiConfig } from './modules/wifiConfig';
import { awaitWebApp } from './tools/awaitWebApp.ts';

awaitWebApp()
  .then((openedFromWebapp) => {
    const mainNode = document.querySelector('.main') as HTMLElement;
    const footerNode = document.querySelector('.footer') as HTMLElement;

    if (!openedFromWebapp) {
      mainNode.appendChild(dumps());
      mainNode.appendChild(wifiConfig());
      footerNode.appendChild(footer());
    } else {
      // ToDo: add comms logic with webapp
    }
  });
