#include <emscripten/emscripten.h>

int main() {
    EM_ASM({
        console.log("C++ main ran");

        let el = document.getElementById("app");
        if (el) {
            el.innerText = "BIO WASM RUNNING";
        } else {
            console.log("No #app element found");
        }
    });

    return 0;
}
