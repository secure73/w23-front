<script>
    const form = document.getElementById("myForm");
    const name = form.elements["name"];
    const email = form.elements["email"];
    const message = form.elements["message"];

    form.addEventListener("submit", function(event) {
      event.preventDefault(); // Отменяем отправку формы по умолчанию

      // Проверяем заполненность полей
      if (!name.value || !email.value || !message.value) {
        alert("Пожалуйста, заполните все поля");
        return;
      }

      // Отправляем данные формы на сервер
      const xhr = new XMLHttpRequest();
      xhr.open("POST", "url_to_server_script.php");
      xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
      xhr.onreadystatechange = function() {
        if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
          alert("Сообщение отправлено");
          form.reset(); // Очищаем форму
        }
      };
      xhr.send("name=" + name.value + "&email=" + email.value + "&message=" + message.value);
    });
  </script>