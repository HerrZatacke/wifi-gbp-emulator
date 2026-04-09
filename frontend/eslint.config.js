import js from '@eslint/js';
import { defineConfig } from 'eslint/config';
import importPlugin from 'eslint-plugin-import';
import globals from 'globals';
import tseslint from 'typescript-eslint';

const eslintConfig = defineConfig([
  js.configs.recommended,
  ...tseslint.configs.recommended,

  {
    files: ['**/*.{js,ts,mjs,mts}'],
    languageOptions: {
      globals: globals.browser,
    },
  },

  {
    files: ['scripts/**/*.{js,ts,mjs,mts}'],
    languageOptions: {
      globals: globals.node,
    },
  },

  {
    plugins: {
      import: importPlugin,
    },
    files: ['**/*.{js,ts,mjs,mts}'],
    rules: {
      'quotes': ['error', 'single'],
      'object-curly-spacing': ['error', 'always'],
      'comma-dangle': ['error', 'always-multiline'],
      'semi': ['error', 'always'],
      'array-bracket-spacing': ['error', 'never'],
      'no-shadow': 'off',
      '@typescript-eslint/no-shadow': ['error'],
      'no-unused-vars': 'off',
      '@typescript-eslint/no-unused-vars': ['warn', { argsIgnorePattern: '^_' }],
      'no-bitwise': ['error'],

      'import/order': [
        'error',
        {
          groups: ['builtin', 'external', 'internal', 'parent', 'sibling', 'index'],
          alphabetize: {
            order: 'asc',
            caseInsensitive: true,
          },
          'newlines-between': 'never',
        },
      ],
      'import/consistent-type-specifier-style': ['error', 'prefer-inline'],
    },
  },
]);

export default eslintConfig;
