interface GenericButtonProps {
  title: string;
  onClick: (ev: MouseEvent) => Promise<void> | void;
  className?: string;
}

export const genericButton = ({
  title,
  onClick,
  className,
}: GenericButtonProps): HTMLButtonElement => {
  const button = document.createElement('button');
  if (className) {
    button.className = className;
  }
  button.innerHTML = title;
  button.addEventListener('click', onClick);
  return button;
};
