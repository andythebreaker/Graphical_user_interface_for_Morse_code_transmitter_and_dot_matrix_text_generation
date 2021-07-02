
$(function () {
    init(after_init());
});
function hexc(colorval) {
    var parts = colorval.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
    delete (parts[0]);
    for (var i = 1; i <= 3; ++i) {
        parts[i] = parseInt(parts[i]).toString(16);
        if (parts[i].length == 1) parts[i] = '0' + parts[i];
    }
    color = '#' + parts.join('');
    return color;
}
function init(callback) {
    document.getElementById("style_width_280px").style.width = "25vw";
    document.getElementById("width_1_5rem").style.width = "1vw";
    for (let index = 0; index < document.getElementById("what_to_snap_body").getElementsByClassName("row_class").length; index++) {
        const element = document.getElementById("what_to_snap_body").getElementsByClassName("row_class")[index];
        for (let index2 = 0; index2 < element.getElementsByClassName("square").length; index2++) {
            const element2 = element.getElementsByClassName("square")[index2];
            element2.id = "cell_numb_" + String(index) + String(index2);
            //element2.innerText = String(index) + String(index2);
        }
    }
    // 如果 callback 是個函式就呼叫它
    if (typeof callback === 'function') {
        callback();
    }
}
function after_init() {
    $('.square').click(function (event) {
        //document.getElementById("get_infoV").innerText = hexc(String($(this).css("background-color")));
        $(this).css("background-color", (hexc(String($(this).css("background-color"))) === "#ddb98b") ? "blue" : "#ddb98b");
    });
    $('.bton_data').click(function (event) {
        document.getElementById("get_infoV").innerText="";
        for (let index = 0; index < document.getElementById("what_to_snap_body").getElementsByClassName("row_class").length; index++) {
            const element = document.getElementById("what_to_snap_body").getElementsByClassName("row_class")[index];
            var int_row_hex=0;
            for (let index2 = 0; index2 < element.getElementsByClassName("square").length; index2++) {
                element2_id = "cell_numb_" + String(index) + String(index2);
                int_row_hex+=(hexc(String($(`#${element2_id}`).css("background-color"))) === "#ddb98b") ? '0':'1';
            }

        }
    });
}
