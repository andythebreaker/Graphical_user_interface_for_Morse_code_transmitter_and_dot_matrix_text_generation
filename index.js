
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
function date_time_now() {
    var months = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];
    var days = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
    var d = new Date();
    var day = days[d.getDay()];
    var hr = d.getHours();
    var min = d.getMinutes();
    if (min < 10) {
        min = "0" + min;
    }
    var ampm = "am";
    if (hr > 12) {
        hr -= 12;
        ampm = "pm";
    }
    var date = d.getDate();
    var month = months[d.getMonth()];
    var year = d.getFullYear();
    return (day + "_" + hr + "_" + min + ampm + "_" + date + "_" + month + "_" + year);
}
function init(callback) {
    document.getElementById("style_width_280px").style.width = "25vw";
    document.getElementById("width_1_5rem").style.width = "1vw";
    for (let index = 0; index < document.getElementById("what_to_snap_body").getElementsByClassName("row_class").length; index++) {
        const element = document.getElementById("what_to_snap_body").getElementsByClassName("row_class")[index];
        for (let index2 = 0; index2 < element.getElementsByClassName("square").length; index2++) {
            const element2 = element.getElementsByClassName("square")[index2];
            element2.id = "cell_numb_" + String(index) + String(index2);
            element2.setAttribute("onclick", `square_click_function_event('${element2.id}')`);
            //element2.innerText = String(index) + String(index2);
        }
    }
    // 如果 callback 是個函式就呼叫它
    if (typeof callback === 'function') {
        callback();
    }
}
function after_init() {
    //$('#obj_all_data_to_save').click(function () {
    /*$('.square').click(function (event) {
        //document.getElementById("get_infoV").innerText = hexc(String($(this).css("background-color")));
        $(this).css("background-color", (hexc(String($(this).css("background-color"))) === "#ddb98b") ? "blue" : "#ddb98b");
    });*/
    //});
    $('.bton_data').click(function (event) {
        var obj_all_data_to_save = [];
        document.getElementById("ascii_row_pattern_hex").innerText = "";
        document.getElementById("ascii_row_pattern_hex_with_data_structure").innerText = "{";
        document.getElementById("block_data_txt").innerText = "<div>";
        //document.getElementById("ascii_row_pattern_hex_with_data_structure_define").innerText = "";
        for (let index = 0; index < document.getElementById("what_to_snap_body").getElementsByClassName("row_class").length; index++) {
            const element = document.getElementById("what_to_snap_body").getElementsByClassName("row_class")[index];
            var int_row_hex = 0;
            var obj_all_data_to_save_sub = [];
            for (let index2 = 0; index2 < element.getElementsByClassName("square").length; index2++) {
                element2_id = "cell_numb_" + String(index) + String(index2);
                int_row_hex += (hexc(String($(`#${element2_id}`).css("background-color"))) === "#ddb98b") ? 0 : Math.pow(2, index2);
                obj_all_data_to_save_sub.push((hexc(String($(`#${element2_id}`).css("background-color"))) === "#ddb98b") ? false : true);
                document.getElementById("block_data_txt").innerText += (hexc(String($(`#${element2_id}`).css("background-color"))) === "#ddb98b") ? "□" : "■";
            }
            document.getElementById("block_data_txt").innerText += "</div><div>"
            obj_all_data_to_save.push(obj_all_data_to_save_sub);
            document.getElementById("ascii_row_pattern_hex").innerText += int_row_hex.toString(16);
            document.getElementById("ascii_row_pattern_hex_with_data_structure").innerText += ("0x" + ((int_row_hex < 10) ? "0" : "") + int_row_hex.toString(16).toUpperCase() + ",");
        }
        document.getElementById("ascii_row_pattern_hex_with_data_structure").innerText = document.getElementById("ascii_row_pattern_hex_with_data_structure").innerText.slice(0, -1) + "}";
        document.getElementById("obj_all_data_to_save").innerText = JSON.stringify(obj_all_data_to_save);
        document.getElementById("bton_data_fake").innerText = "1";
        document.getElementById("block_data_txt").innerText = document.getElementById("block_data_txt").innerText.slice(0, -1).slice(0, -1).slice(0, -1).slice(0, -1).slice(0, -1);
    });
    document.getElementById('btn_index_text').addEventListener('click', () => {
        document.getElementById("bton_data_fake").innerText = "-1";
        document.getElementById("bton_data_fake").click();
        function checkFlag() {
            if (document.getElementById('bton_data_fake').innerText !== '1') {
                setTimeout(() => {
                    checkFlag();
                }, 5);
            } else {
                //document.getElementById('test_01').innerText=`${document.getElementById('what_to_snap_body').innerHTML.replace(/"/g,'~').replace(/\n/g,'').trim().replace(/&nbsp;/g, '').replace(/[^\S\r\n]{2,}/g, '')}`;
                $('#val_index_text').val((!$('#val_index_text').val()) ? date_time_now() : $('#val_index_text').val());
                document.getElementById("all_commits").innerHTML = document.getElementById("all_commits").innerHTML + `<tr class="index_count class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')}">
<th class="class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')}" scope="col">${document.getElementById("index_number_save_max").innerText.replace('_index','')}</th>
<th scope="col" class="class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')} data_save_table_val_index_text" id="${ document.getElementById("index_number_save_max").innerText.replace('_index','')}_data_save_table_val_index_text">${$('#val_index_text').val()}</th>
<th style="font-family: 'SourceHanSansTC-VF';" class="class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')}" scope="col"><div class="class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')} pure_text_pattern" style="line-height:0.7rem;">
${document.getElementById("block_data_txt").innerText}
</div></th>
<th scope="col" class="class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')}">
<button onclick="trigger_load_event_click_event('${ document.getElementById("index_number_save_max").innerText.replace('_index','')}_trigger_load')" value="${document.getElementById('what_to_snap_body').innerHTML.replace(/"/g, '~').replace(/\n/g, '').trim().replace(/&nbsp;/g, '').replace(/[^\S\r\n]{2,}/g, '')}" 
type="button" class="btn btn-primary trigger_load_event class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')}" id="${ document.getElementById("index_number_save_max").innerText.replace('_index','')}_trigger_load">load</button>
</th><th scope="col">
<button onclick="trigger_del_event_click_event('${ document.getElementById("index_number_save_max").innerText.replace('_index','')}_trigger_del')"
type="button" class="btn btn-warning trigger_del_event class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')}" id="${ document.getElementById("index_number_save_max").innerText.replace('_index','')}_trigger_del">del</button>
</th>
<th scope="col" class="class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')}">${document.getElementById("ascii_row_pattern_hex").innerText}</th>
<th scope="col" class="class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')}">${document.getElementById("ascii_row_pattern_hex_with_data_structure").innerText}</th>
<th scope="col" class="class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')}">#define ASCII88PATTERN_${$('#val_index_text').val()} ${document.getElementById("ascii_row_pattern_hex_with_data_structure").innerText}</th>
<th scope="col" class="class_for_remove_${document.getElementById("index_number_save_max").innerText.replace('_index','')}">${document.getElementById("obj_all_data_to_save").innerText}</th>
</tr>`;
            }
            document.getElementById("index_number_save_max").innerText=String(parseInt(document.getElementById("index_number_save_max").innerText.replace('_index',''),10)+1)+'_index';
        }
        checkFlag();
    });
    document.getElementById("agent_del_fucn").addEventListener("click", () => {
        $('.'+document.getElementById("agent_del_fucn").innerText).remove();
    });
}

function trigger_load_event_click_event(target_id) {
    document.getElementById('what_to_snap_body').innerHTML = document.getElementById(target_id).value.replace(/~/g, '"');
    $("#val_index_text").val(document.getElementById(target_id.replace('_trigger_load', '')+'_data_save_table_val_index_text').innerText);
    //document.getElementById('obj_all_data_to_save').click();
}

function square_click_function_event(string_id) {
    $(`#${string_id}`).css('background-color', (hexc(String($(`#${string_id}`).css('background-color'))) === '#ddb98b') ? 'blue' : '#ddb98b');
}
function trigger_del_event_click_event(id_string) {
    document.getElementById("agent_del_fucn").innerText = 'class_for_remove_'+id_string.replace('_trigger_del','');
    document.getElementById("agent_del_fucn").click();
}