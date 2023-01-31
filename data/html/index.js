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

function createGuage(elAppendix, valueText, description, value, size) {
    app.gauge.create({
        el: '.gauge_template_' + elAppendix,
        type: 'circle',
        value: value,
        size: size,
        borderColor: '#2196f3',
        borderWidth: 10,
        valueText: valueText,
        valueFontSize: 41,
        valueTextColor: '#2196f3',
        labelText: description,
    })
}

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
