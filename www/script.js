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

// POST
function sendPostRequest() {
  const file = document.getElementById("fileName").value;
  const data = document.getElementById("postData").value;
  if (!file || !data) {
    alert("Veuillez entrer des données !");
    return;
  }
  const message = `file=${file} data=${data}`;
  fetch("/", {
    method: "POST",
    headers: {
      "Content-Type": "text/plain",
    },
    body: message,
  })
    .then((response) => response.text())
    .then((html) => {
      document.getElementById("postResponse").innerText = html;
    })
    .catch((error) => console.error("Erreur lors de l'envoi :", error));
}

// POST CGI
function sendPostRequestNewWindow() {
  let data = document.getElementById("postDataCGI").value;
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
      .then((response) => response.text())
      .then((html) => {
        const parser = new DOMParser();
        const doc = parser.parseFromString(html, "text/html");

        const fileList = document.getElementById("fileList");
        fileList.innerHTML =
          '<option value="">Sélectionner un fichier</option>';
        const links = doc.querySelectorAll("a");
        links.forEach((link) => {
          const fileName = link.textContent.trim();
          if (fileName !== ".." && fileName !== "." && fileName !== "") {
            let option = document.createElement("option");
            option.value = fileName;
            option.textContent = fileName;
            fileList.appendChild(option);
          }
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
      .then((response) => response.text())
      .then((html) => {
        document.getElementById("deleteResponse").textContent = html;
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
  fetch("cgi/cookie.py", {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
    body: data,
  })
    .then((response) => response.json())
    .then((text) => {
      console.log("Cookie activé");
      document.getElementById("cookieStatus").textContent = "Cookie activés";
      updateVisitCount();
    })
    .catch((error) => console.error("Erreur:", error));
}

function updateVisitCount() {
  fetch("/cgi/cookie.py")
    .then((response) => response.json())
    .then((data) => {
      if (data.visits !== "Cookies désactivés") {
        document.getElementById("visitCount").textContent = data.visits;
      } else {
        document.getElementById("visitCount").textContent = "";
      }
    })
    .catch((error) => console.error("Erreur:", error));
}

document.addEventListener("DOMContentLoaded", function () {
  if (document.cookie.includes("session_id")) {
    updateVisitCount();
    document.getElementById("visitCount").style.display = "block";
  } else {
    document.getElementById("visitCount").style.display = "none";
  }
});
