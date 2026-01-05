module.exports = {
  technicalSidebar: [
    'technical-docs/server/overview',
    'technical-docs/server/network-protocol',
    {
      type: 'category',
      label: 'Client',
      items: [
        'technical-docs/client/audio/audio_doc',
        'technical-docs/client/engine/game_loop',
        'technical-docs/client/engine/client_architecture',
        'technical-docs/client/graphic/graphic_doc',
        'technical-docs/client/resources/assets_and_levels',
        'technical-docs/client/ui/menus_and_navigation',
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
            'technical-docs/server/engine/game-server',
            'technical-docs/server/engine/systems',
            'technical-docs/server/engine/world',
            'technical-docs/server/engine/runtime-integration',
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
