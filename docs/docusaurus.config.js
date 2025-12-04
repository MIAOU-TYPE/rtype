export default {
  title: 'R-TYPE Engine',
  tagline: 'A Game Engine That Roars!',
  url: 'https://rtype-game.github.io',
  baseUrl: '/',
  favicon: 'img/favicon.ico',

  organizationName: 'rtype-team',
  projectName: 'rtype',

  presets: [
    [
      'classic',
      {
        docs: {
          path: './docs',
          routeBasePath: '/docs',
          sidebarPath: './sidebars.js',
        },
        theme: {
          customCss: './src/css/custom.css',
        },
      },
    ],
  ],

  themeConfig: {
    navbar: {
      title: 'R-TYPE',
      items: [
        {
          label: 'Technical Documentation',
          to: '/docs/technical-docs/server/overview',
          position: 'left',
        },
        {
          label: 'User Guide',
          to: '/docs/user-guide/overview',
          position: 'left',
        },
        {
          label: 'How to Contribute',
          to: '/docs/contributing/overview',
          position: 'left',
        },
        {
          href: 'https://github.com/rtype-team/r-type',
          label: 'GitHub',
          position: 'right',
        },
      ],
    },
  },
};
