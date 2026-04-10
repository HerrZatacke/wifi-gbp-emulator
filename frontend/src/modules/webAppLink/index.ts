
export const webAppLink = (className: string): HTMLAnchorElement => {
  const moduleRoot = document.createElement('a');
  moduleRoot.className = className;
  moduleRoot.innerText = 'Open in Web-App';
  moduleRoot.target = '_self';
  moduleRoot.href = `https://herrzatacke.github.io/gb-printer-web/import/?addPrinter=${encodeURIComponent(window.origin)}`;
  return moduleRoot;
};
