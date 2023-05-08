<script>
    const form = document.getElementById("myForm");
    const name = form.elements["name"];
    const email = form.elements["email"];
    const message = form.elements["message"];

    form.addEventListener("submit", function(event) {
      event.preventDefault(); // Stopp das Standardverhalten des Formulars

      // Überprüfen Sie, ob alle Felder ausgefüllt sind
      if (!name.value || !email.value || !message.value) {
        alert("AAAAAA, bitte füllen Sie alle Felder aus");
        return;
      }

      // Senden Sie die Formulardaten an den Server
      const xhr = new XMLHttpRequest();
      xhr.open("POST", "url_to_server_script.php");
      xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
      xhr.onreadystatechange = function() {
        if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
          alert("Nachricht gesendet");
          form.reset(); // Leeren Sie das Formular
        }
      };
      xhr.send("name=" + name.value + "&email=" + email.value + "&message=" + message.value);
    });
  </script>