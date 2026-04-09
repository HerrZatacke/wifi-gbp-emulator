import 'reset-css/reset.css';
import './styles/style.scss';
import { dumps } from './modules/dumps';
import { footer } from './modules/footer';
import { remoteMessage } from './modules/remoteMessage';
import { wifiConfig } from './modules/wifiConfig';
import initCommands from './remote/initCommands.ts';
import startHeartbeat from './remote/startHeartbeat.ts';
import { awaitWebApp } from './tools/awaitWebApp.ts';

awaitWebApp()
  .then(({ isRemote, targetWindow }) => {
    const mainNode = document.querySelector('.main') as HTMLElement;
    const footerNode = document.querySelector('.footer') as HTMLElement;

    if (!isRemote || !targetWindow) {
      mainNode.appendChild(dumps());
      mainNode.appendChild(wifiConfig());
      footerNode.appendChild(footer());
    } else {
      mainNode.appendChild(remoteMessage());
      const printerCommands = initCommands(targetWindow);
      startHeartbeat(targetWindow, printerCommands.map(({ name }) => name));
    }
  });
