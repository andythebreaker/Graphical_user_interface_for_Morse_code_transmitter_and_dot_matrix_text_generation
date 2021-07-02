
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
        var obj_all_data_to_save=[];
        document.getElementById("ascii_row_pattern_hex").innerText="";
        document.getElementById("ascii_row_pattern_hex_with_data_structure").innerText="{";
        document.getElementById("ascii_row_pattern_hex_with_data_structure_define").innerText="";
        for (let index = 0; index < document.getElementById("what_to_snap_body").getElementsByClassName("row_class").length; index++) {
            const element = document.getElementById("what_to_snap_body").getElementsByClassName("row_class")[index];
            var int_row_hex=0;
            var obj_all_data_to_save_sub=[];
            for (let index2 = 0; index2 < element.getElementsByClassName("square").length; index2++) {
                element2_id = "cell_numb_" + String(index) + String(index2);
                int_row_hex+=(hexc(String($(`#${element2_id}`).css("background-color"))) === "#ddb98b") ? 0:Math.pow(2,index2);
                obj_all_data_to_save_sub.push((hexc(String($(`#${element2_id}`).css("background-color"))) === "#ddb98b") ? false:true);
            }
            obj_all_data_to_save.push(obj_all_data_to_save_sub);
            document.getElementById("ascii_row_pattern_hex").innerText+=int_row_hex.toString(16);
            document.getElementById("ascii_row_pattern_hex_with_data_structure").innerText+=("0x"+int_row_hex.toString(16)+",");
        }slice(0, -1)
        document.getElementById("ascii_row_pattern_hex_with_data_structure").innerText=document.getElementById("ascii_row_pattern_hex_with_data_structure").innerText.slice(0, -1).slice(0, -1)+"}";
        document.getElementById("ascii_row_pattern_hex_with_data_structure_define").innerText=JSON.stringify(obj_all_data_to_save);
    });
}
