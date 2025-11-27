module.exports = {
  technicalSidebar: [
    'technical-docs/overview',
    {
      type: 'category',
      label: 'Engine',
      items: [
        'technical-docs/engine/overview',
        'technical-docs/engine/ecs',
        'technical-docs/engine/rendering',
        'technical-docs/engine/audio',
        'technical-docs/engine/physics',
        'technical-docs/engine/ai',
      ],
    },
    {
      type: 'category',
      label: 'Networking',
      items: [
        'technical-docs/network',
      ],
    },
    {
      type: 'category',
      label: 'Gameplay',
      items: [
        'technical-docs/gameplay',
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
