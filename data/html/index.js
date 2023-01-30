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
    // Add default routes,
    autoDarkMode: true,
    routes: [
        {
            path: '/about/',
            url: 'about.html',
        },
    ]
});

function doLogin(salt) {
    let data = app.form.convertToData("#loginForm")
    if (data.username.length == 0) {
        app.dialog.alert("请输入用户名")
        return
    }
    if (data.password.length == 0) {
        app.dialog.alert("请输入密码")
        return
    }
    data.password = sha256_digest(data.password + salt)
    window.location = "/login?v=" + JSON.stringify(data)
    return
}
