window.addEventListener("DOMContentLoaded", async () => {
    const partials = [
      "get-requests.html",
      "post-request.html",
      "delete-request.html",
      "upload.html",
      "cookies.html"
    ];
  
    const container = document.getElementById("content");
  
    for (const file of partials) {
      const res = await fetch(`partials/${file}`);
      const html = await res.text();
      container.innerHTML += html;
    }
  });
  