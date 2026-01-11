module.exports = {
  technicalSidebar: [
    {
      type: 'category',
      label: 'Client',
      items: [
        'technical-docs/client/overview',
        {
          type: 'category',
          label: 'Architecture',
          items: [
            'technical-docs/client/architecture/overview',
            'technical-docs/client/architecture/game-loop',
            'technical-docs/client/architecture/client-runtime',
          ],
        },
        {
          type: 'category',
          label: 'Network',
          items: [
            'technical-docs/client/network/overview',
            'technical-docs/client/network/tcp',
            'technical-docs/client/network/udp',
            'technical-docs/client/network/state-sync',
          ],
        },
        {
          type: 'category',
          label: 'Auth',
          items: [
            'technical-docs/client/auth/overview',
            'technical-docs/client/auth/flows',
            'technical-docs/client/auth/tcp-protocol',
            'technical-docs/client/auth/udp-bind',
            'technical-docs/client/auth/persistence',
            'technical-docs/client/auth/ui-events',
          ],
        },
        {
          type: 'category',
          label: 'UI',
          items: [
            'technical-docs/client/ui/overview',
            'technical-docs/client/ui/menus-and-navigation',
          ],
        },
        {
          type: 'category',
          label: 'Graphics',
          items: [
            'technical-docs/client/graphics/overview',
            'technical-docs/client/graphics/rendering',
          ],
        },
        {
          type: 'category',
          label: 'Audio',
          items: [
            'technical-docs/client/audio/overview',
            'technical-docs/client/audio/audio',
          ],
        },
        {
          type: 'category',
          label: 'Resources',
          items: [
            'technical-docs/client/resources/overview',
            'technical-docs/client/resources/assets',
            'technical-docs/client/resources/levels',
          ],
        },
        {
          type: 'category',
          label: 'Events',
          items: [
            'technical-docs/client/events/overview',
            'technical-docs/client/events/event-bus',
          ],
        },
      ],
    },
    {
      type: 'category',
      label: 'Server',
      items: [
        'technical-docs/server/overview',
        {
          type: 'category',
          label: 'Architecture',
          items: [
            'technical-docs/server/architecture/overview',
            'technical-docs/server/architecture/runtime',
            'technical-docs/server/architecture/game-loop',
          ],
        },
        {
          type: 'category',
          label: 'Network',
          items: [
            'technical-docs/server/network/overview',
            'technical-docs/server/network/tcp',
            'technical-docs/server/network/udp',
            'technical-docs/server/network/protocol',
          ],
        },
        {
          type: 'category',
          label: 'Auth',
          items: [
            'technical-docs/server/auth/auth-overview',
            'technical-docs/server/auth/auth-database',
            'technical-docs/server/auth/auth-password-hashing',
            'technical-docs/server/auth/auth-sessions-udp',
            'technical-docs/server/auth/auth-tcp-api',
          ],
        },
        {
          type: 'category',
          label: 'Resources',
          items: [
            'technical-docs/server/resources/overview',
            'technical-docs/server/resources/config',
            'technical-docs/server/resources/persistence',
          ],
        },
        {
          type: 'category',
          label: 'Events',
          items: [
            'technical-docs/server/events/overview',
            'technical-docs/server/events/event-bus',
          ],
        },
        {
          type: 'category',
          label: 'Engine',
          items: [
            'technical-docs/server/engine/overview',
            'technical-docs/server/engine/ecs',
            'technical-docs/server/engine/systems',
            'technical-docs/server/engine/world',
            'technical-docs/server/engine/game-server',
            'technical-docs/server/engine/runtime-integration',
          ],
        },
      ],
    },
  ],
  userGuideSidebar: [
    'user-guide/overview',
    'user-guide/installation',
    'user-guide/usage',
    'user-guide/tips',
    'user-guide/mechanics_and_roles',
  ],
  contributingSidebar: [
    'contributing/overview',
    'contributing/guidelines',
    'contributing/pull-requests',
    'contributing/add-missile-sprite',
  ],
};