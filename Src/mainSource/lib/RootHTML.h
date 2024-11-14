#ifndef RootHTML_H
#define RootHTML_H

String html = "<!DOCTYPE html><html lang='es'>"
                "<head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                "<title>Configuraci&oacute;n WiFi</title>"
                "<style>"
                "body{font-family:system-ui,-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;color:#1d1d1f;background-color:#f5f5f7;margin:0;padding:20px;display:flex;align-items:center;justify-content:center;min-height:100vh}"
                ".container{background-color:#fff;padding:30px;border-radius:12px;box-shadow:0 4px 6px rgba(0,0,0,0.1);max-width:300px;width:100%}"
                "h1{font-size:24px;text-align:center;margin-bottom:20px;color:#1d1d1f}"
                "form{display:flex;flex-direction:column}"
                "input{width:100%;padding:10px;margin-bottom:15px;border:1px solid #d2d2d7;border-radius:6px;font-size:14px}"
                "input[type='submit']{background-color:#0071e3;color:white;border:none;font-weight:bold;cursor:pointer}"
                "input[type='submit']:hover{background-color:#0077ed}"
                "</style>"
                "</head>"
                "<body>"
                "<div class='container'>"
                "<h1>EMM ETEC</h1>"
                "<h1>Configuraci&oacute;n WiFi</h1>"
                "<form action='/save' method='post'>"
                "<input type='text' name='ssid' placeholder='Nombre de la red (SSID)' required>"
                "<input type='password' name='password' placeholder='Contrase&ntilde;a' required>"
                "<input type='submit' value='Guardar'>"
                "</form>"
                "</div>"
                "</body>"
                "</html>";

#endif
