import './index.scss';

interface HeadlineProps {
  level: 1 | 2; // |3|4
  text: string;
}

export const headline = ({ level, text }: HeadlineProps): HTMLHeadingElement => {
  const moduleRoot = document.createElement(`h${level}`);
  moduleRoot.className = `headline headline--level${level}`;
  moduleRoot.innerText = text;
  return moduleRoot;
};
