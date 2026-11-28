# puts rsa.wasm and the fonts inside the page so rsa_ui.html is one file that works offline
import base64
def b64(path):
    return base64.b64encode(open(path, "rb").read()).decode()
page = open("index.template.html", encoding="utf-8").read()
page = page.replace("__WASM_B64__", b64("rsa.wasm"))
page = page.replace("__FONT_STENCIL__", b64("fonts/stencil.woff2"))
page = page.replace("__FONT_BARLOW__", b64("fonts/barlow600.woff2"))
page = page.replace("__FONT_COURIER400__", b64("fonts/courier400.woff2"))
page = page.replace("__FONT_COURIER700__", b64("fonts/courier700.woff2"))
open("rsa_ui.html", "w", encoding="utf-8").write(page)
print("rsa_ui.html written,", len(page), "bytes")
