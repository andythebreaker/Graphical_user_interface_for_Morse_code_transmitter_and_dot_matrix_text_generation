
$(function () {
    init(after_init());
});
function init(callback) {
    document.getElementById("style_width_280px").style.width = "25vw";
    document.getElementById("width_1_5rem").style.width = "1vw";
    for (let index = 0; index < document.getElementById("what_to_snap_body").getElementsByClassName("row_class").length; index++) {
        const element = document.getElementById("what_to_snap_body").getElementsByClassName("row_class")[index];
        for (let index2 = 0; index2 < element.getElementsByClassName("square").length; index2++) {
            const element2 = element.getElementsByClassName("square")[index2];
            element2.id = String(element.id) + String(index2);
            //element2.innerText = String(index) + String(index2);
        }
    }
    // 如果 callback 是個函式就呼叫它
    if (typeof callback === 'function') {
        callback();
    }
}
function after_init() {
    $('.square::before').click(function () {
        $(this).css("background-color", "blue");
    });
}
