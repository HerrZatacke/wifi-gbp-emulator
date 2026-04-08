import './index.scss';

interface GenericInputProps {
  title: string;
  initialValue: string;
  type?: string;
  className?: string;
  readonly?: boolean;
}

export interface GenericInput {
  input: HTMLInputElement;
  node: HTMLLabelElement;
}

export const genericInput = ({
  title,
  initialValue,
  type,
  className,
  readonly,
}: GenericInputProps): GenericInput => {
  const node = document.createElement('label');
  node.className = `generic-input ${className}`;
  const labelSpan = document.createElement('span');
  labelSpan.className = 'generic-input__label';
  const input = document.createElement('input');
  input.className = 'generic-input__input';
  labelSpan.innerHTML = title;
  input.value = initialValue;
  input.type = type || 'text';
  input.readOnly = readonly || false;

  input.addEventListener('blur', () => {
    input.value = input.value.trim();
  });

  node.appendChild(labelSpan);
  node.appendChild(input);

  return {
    input,
    node,
  };
};
