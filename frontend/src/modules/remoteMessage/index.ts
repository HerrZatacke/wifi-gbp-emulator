import './index.scss';

const renderModule = async (moduleRoot: HTMLElement) => {
  moduleRoot.innerHTML = `
<p>This is your Wi-Fi printer’s remote control page.</p>
<p>Running on ${window.location.host}</p>
<p>You can go back to your main app and start importing your images.</p>
<p><em class="remote-message__em">In order for the import to work, this window needs to remain open!</em></p>
<p><a href="${window.location.href}" target="_blank" rel="noopener">You can also interact with the printer emulator directly</a></p>
`;
};

export const remoteMessage = (): HTMLElement => {
  const moduleRoot = document.createElement('div');
  moduleRoot.className = 'remote-message';
  renderModule(moduleRoot);
  return moduleRoot;
};
