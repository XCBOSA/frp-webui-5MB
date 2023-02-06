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

function isValidIP(ip) {
    let reg = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/
    return reg.test(ip)
}

function doModify(uuid) {
    let data = app.form.convertToData("#modifyForm_" + uuid)
    if (!isValidIP(data.address)) {
        app.dialog.alert("IP地址格式错误，请重新填写。")
        return
    }
    if (data.port < 0 && data.port >= 65536) {
        app.dialog.alert("内部端口号必须在0-65535范围内。")
        return
    }
    window.location = "/changePortSettings?v=" + JSON.stringify(data)
    return
}

function doCreate() {
    let data = app.form.convertToData("#createNewForm")
    if (data.address == "") {
        app.dialog.alert("请填写内部IP地址。")
        return
    }
    if (!isValidIP(data.address)) {
        app.dialog.alert("IP地址格式错误，请重新填写。")
        return
    }
    if (data.port == "") {
        app.dialog.alert("请填写内部端口。")
        return
    }
    if (data.port < 0 && data.port >= 65536) {
        app.dialog.alert("内部端口号必须在0-65535范围内。")
        return
    }
    window.location = "/createPort?v=" + JSON.stringify(data)
    return
}

function doDelete(uuid) {
    let req = {
        "uuid": uuid
    }
    window.location = "/removePort?v=" + JSON.stringify(req)
}
