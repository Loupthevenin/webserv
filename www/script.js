async function sendGetRequest() {
  const response = await fetch("/methods/GET.html", { method: "GET" });
  const data = await response.text();
  document.getElementById("getResponse").innerText = data;
}

function sendGetRequestNewWindow() {
  window.location.href = "/methods/GET.html";
}

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
