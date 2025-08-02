### **Об проекте**  

WebView2 desktop sample - это шаблон приложения для Windows на C++, который демонстрирует интеграцию современного веб-интерфейса в нативное десктопное приложение с использованием Microsoft Edge WebView2.

**Проект предоставляет из себя**
- Готовую инфраструктуру для работы с WebView2 и встраивания веб-контента 
- Двустороннюю связь между JavaScript и C++
- Пример встраивания веб-ресурсов (HTML, CSS, JS) в исполняемый файл
- Интеграцию с популярными C++ библиотеками (fmt, spdlog, cpprestsdk)
- Идеально подходит для быстрого старта при разработке гибридных приложений, где требуется сочетание производительности C++ и гибкости веб-технологий.


### **Этапы сборки**  

**1. Предварительные требования**  
Перед сборкой необходимо установить и настроить следующие инструменты:  
- **[vcpkg](https://github.com/microsoft/vcpkg)** – менеджер C++ библиотек (убедитесь, что `vcpkg.exe` доступен через `PATH`)  
- **[NuGet](https://www.nuget.org/downloads)** – система управления пакетами для Windows (`nuget.exe` должен быть в `PATH`)  
- **[Node.js](https://nodejs.org/)** (LTS-версия) + **Vue CLI** (`npm install -g @vue/cli`)  

**2. Клонирование репозитория**  
```
git clone https://github.com/waparabka/webview2-desktop-sample
cd webview2-desktop-sample
```

**3. Подготовка веб-интерфейса (Vue.js)**  
```
cd third-party
vue create ui  # Создаем Vue-проект в папке `ui`
cd ui
npm run build # Сборка веб-приложения (появится в `third-party/ui/dist`)
```

**4. Настройка веб-интерфейса (Vue.js)**  
Очень важно, что бы vue.config.js выглядел так (если используете интерфейс из ресурсов проекта)
```
const { defineConfig } = require('@vue/cli-service')
module.exports = defineConfig({
  transpileDependencies: true,
  publicPath: 'res://'
})
```

**5. Сборка C++ приложения**  
```
cd ../..  # Возвращаемся в корень проекта
cmake -B project -A Win32  # Генерация проекта для Visual Studio
cmake --build project --config Release  # Компиляция (или открываем .sln в Visual Studio)
```

**6. Запуск**  
После успешной компиляции исполняемый файл будет доступен в:  
```
project/Release/w2ds.exe
```

Готовое приложение работает как самостоятельный EXE-файл без внешних зависимостей (статическая линковка).
