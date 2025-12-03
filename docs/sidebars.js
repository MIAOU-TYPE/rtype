module.exports = {
  technicalSidebar: [
    'technical-docs/server/overview',
    {
      type: 'category',
      label: 'Client',
      items: [
        'technical-docs/client/graphic/graphic_doc',
        'technical-docs/client/network/client_network_doc',
      ],
    },
    {
      type: 'category',
      label: 'Server',
      items: [
        {
          type: 'category',
          label: 'Engine',
          items: [
            'technical-docs/server/engine/overview',
            'technical-docs/server/engine/ecs',
            'technical-docs/server/engine/rendering',
            'technical-docs/server/engine/audio',
            'technical-docs/server/engine/physics',
            'technical-docs/server/engine/ai',
          ],
        },
        {
          type: 'category',
          label: 'Networking',
          items: [
            'technical-docs/server/network/overview',
            'technical-docs/server/network/udp-server',
            'technical-docs/server/network/netwrapper-and-sockets',
            'technical-docs/server/network/protocol',
          ],
        },
        'technical-docs/server/gameplay',
      ],
    },
  ],
  userGuideSidebar: [
    'user-guide/overview',
    'user-guide/installation',
    'user-guide/usage',
    'user-guide/tips',
  ],
  contributingSidebar: [
    'contributing/overview',
    'contributing/guidelines',
    'contributing/pull-requests',
  ],
};
