
$(function () {
    document.getElementById('pswtest').addEventListener('click', () => {
        console.log($('#pswV').val());
        $.post("/chkPsw", {
            psw: $('#pswV').val()
        }, (res) => {
            document.getElementById('pswtest').classList.remove("btn-primary");
            document.getElementById('pswtest').classList.remove("btn-success");
            document.getElementById('pswtest').classList.remove("btn-danger");
            if (res == "True") {
                document.getElementById('pswtest').classList.add("btn-success");
            } else {
                document.getElementById('pswtest').classList.add("btn-danger");
            }
        });
    });

    document.getElementById("get_info").addEventListener("click", function () {
        $.post("/git_version_info", {
            //empty
        }, (res) => {
            document.getElementById("get_infoV").innerText = res;
        });
    });
    document.getElementById("game_main_website_status").addEventListener("click", function () {
        $.post("/game_main_website_status", {
            //empty
        }, (res) => {
            document.getElementById("game_main_website_statusV").innerText = res;
        });
    });
    document.getElementById("Pull_the_remote_code_to_the_local_end_and_trigger_the_update").addEventListener("click", function () {
        document.getElementById('pswtest').click();
        $.post("/Pull_the_remote_code_to_the_local_end_and_trigger_the_update", {
            psw: $('#pswV').val()
        }, (res) => {
            if (res == "success") {
                console.log("success");
            } else {
                console.error(res);
            }
            fake_reload();
        });
    });
    document.getElementById("all_repo").addEventListener("click", function (e) {
        document.getElementById("all_commits").innerHTML = "";
        $.get("https://api.github.com/repos/UIDD2021ElderlyApp/backend_ex/commits", {
            //empty
        }, (res) => {
            console.log(res)
            for (let index = 0; index < res.length; index++) {
                /*console.log(index);
                console.log(res[index].sha);
                console.log(res[index].commit.author.name);console.log(res[index].commit.author.date);
                console.log(res[index].commit.committer.name);console.log(res[index].commit.committer.date);
                console.log(res[index].commit.message);
                console.log(res[index].html_url);*/
                document.getElementById("all_commits").innerHTML = document.getElementById("all_commits").innerHTML + `<tr>
<th scope="row">${index}</th>
<th scope="row">
<button type="button" class="btn btn-warning trigger_version_change" id="${res[index].sha}_trigger_version_change">Use this version and restart the server</button>
</th>
<th scope="col">${res[index].sha}</th>
<th scope="col">${res[index].commit.author.name}</th>
<th scope="col">${res[index].commit.author.date}</th>
<th scope="col">${res[index].commit.committer.name}</th>
<th scope="col">${res[index].commit.committer.date}</th>
<th scope="col">${res[index].commit.message}</th>
<th scope="col">${res[index].html_url}</th>
</tr>`;
            }
            $('.trigger_version_change').on('click', function (e) {
                console.log(`trigger_version_change-->${e.target.id}`);
                document.getElementById('pswtest').click();
                $.post("/trigger_version_change_git_reset_hard", {
                    trigger_version_change_git_reset_hard_sha: e.target.id.replace("_trigger_version_change", ""),
                    psw: $('#pswV').val()
                }, (res) => {
                    if (res == "success") {
                        console.log("success");
                    } else {
                        console.error(res);
                    }
                    fake_reload();
                });
            });
        });
    });
    init();
});
function init() {
    document.getElementById("style_width_280px").style.width = "25vw";
    document.getElementById("style_RIGHT").style.width = "74vw";
    document.getElementById("width_1_5rem").style.width = "1vw";
    fake_reload();
    /*document.getElementById("get_info").click();
    document.getElementById("all_repo").click();
    document.getElementById("game_main_website_status").click();*/
}
function fake_reload() {
    document.getElementById("get_info").click();
    document.getElementById("all_repo").click();
    document.getElementById("game_main_website_status").click();
    //document.getElementById('pswtest').click();
}