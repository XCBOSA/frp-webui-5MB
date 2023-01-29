let app = new Framework7({
    // App root element
    el: '#app',
    // App Name
    name: 'FRPCWebUI',
    // App id
    id: 'org.forgetive.frpcwebui',
    // Enable swipe panel
    panel: {
        swipe: true,
    },
    // Add default routes
    routes: [
        {
            path: '/about/',
            url: 'about.html',
        },
    ],
    // ... other parameters
});
