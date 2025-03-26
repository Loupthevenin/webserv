// First GET
async function sendGetRequest() {
  const response = await fetch("/methods/GET.html", { method: "GET" });
  const data = await response.text();
  document.getElementById("getResponse").innerText = data;
}

// SECOND GET
function sendGetRequestNewWindow() {
  window.location.href = "/methods/GET.html";
}

// GET CGI
function sendGetCGIRequestNewWindow() {
  window.location.href = "/cgi/cgi.sh";
}

// POST CGI
function sendPostRequestNewWindow() {
  let data = document.getElementById("postData").value;
  if (!data) {
    alert("Veuillez entrer des données !");
    return;
  }
  fetch("/cgi/cgi.sh", {
    method: "POST",
    headers: {
      "Content-Type": "text/plain",
    },
    body: data,
  })
    .then((response) => response.text())
    .then((html) => {
      let newWindow = window.open();
      newWindow.document.write(html);
      newWindow.document.close();
    })
    .catch((error) => console.error("Erreur lors de l'envoi :", error));
}

// EVENT LISTENER FOR FILES IN DELETE
document.addEventListener("DOMContentLoaded", function () {
  fetchFileList();

  function fetchFileList() {
    fetch("/methods/files")
      .then((response) => response.json())
      .then((data) => {
        const fileList = document.getElementById("fileList");
        fileList.innerHTML =
          '<option value="">Sélectionner un fichier</option>';
        data.files.forEach((file) => {
          let option = document.createElement("option");
          option.value = file;
          option.textContent = file;
          fileList.appendChild(option);
        });
      })
      .catch((error) => console.error("Error files delete: ", error));
  }

  window.sendDeleteRequest = function () {
    const fileName = document.getElementById("fileList").value;
    if (!fileName) {
      document.getElementById("deleteResponse").textContent =
        "Sélectionner un fichier.";
      return;
    }

    fetch(`/methods/delete/${fileName}`, {
      method: "DELETE",
    })
      .then((response) => response.json())
      .then((data) => {
        document.getElementById("deleteResponse").textContent = data.message;
        fetchFileList();
      })
      .catch(
        (error) =>
          (document.getElementById("deleteResponse").textContent =
            "Error: " + error),
      );
  };
});

// UPLOADS
document
  .getElementById("uploadForm")
  .addEventListener("submit", function (event) {
    event.preventDefault();

    let formData = new FormData(this);

    fetch("/cgi/uploads.py", {
      method: "POST",
      body: formData,
    })
      .then((response) => response.json())
      .then((data) => {
        if (data.success) {
          document.getElementById("responseMessage").innerHTML = data.message;
          console.log(data);
        } else {
          document.getElementById("responseMessage").innerHTML = data.error;
          console.log(data);
        }
      })
      .catch((error) => {
        console.error("Erreur lors de l’envoi :", error);
        document.getElementById("responseMessage").innerText =
          "Erreur lors du téléversement.";
      });
  });

// COOKIES
function enableCookies() {
  let data = "action=enable";
  fetch("cgi/cookies.py", {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
    body: data,
  })
    .then((response) => response.text())
    .then((text) => {
      document.getElementById("cookieStatus").textContent = text;
      console.log(text);
      console.log("Cookie activé");
    })
    .catch((error) => console.error("Erreur:", error));
}

function disableCookies() {
  let data = "action=disable";
  fetch("cgi/cookies.py", {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
    body: data,
  })
    .then((response) => response.text())
    .then((text) => {
      document.getElementById("cookieStatus").textContent = text;
      console.log("Cookie désactivés");
    })
    .catch((error) => console.error("Erreur:", error));
}
