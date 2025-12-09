# Инструкция по деплою на Firebase

## Проблемы и решения

### Проблема: Сайт не грузит после деплоя

**Причины:**
1. API URL указывает на localhost (не работает в продакшене)
2. Firebase hosting настроен неправильно
3. Бэкенд не задеплоен или недоступен

## Решение

### 1. Настройка переменных окружения

Создайте файл `.env.production` в папке `kursach-react/`:

```env
REACT_APP_API_URL=https://your-backend-url.com/api
```

**Важно:** Замените `https://your-backend-url.com/api` на реальный URL вашего бэкенда!

### 2. Деплой бэкенда

Вам нужно задеплоить Node.js бэкенд на один из сервисов:
- **Heroku** (бесплатный)
- **Railway** (бесплатный)
- **Render** (бесплатный)
- **DigitalOcean** (платный)
- **VPS сервер**

#### Пример для Heroku:

```bash
cd server
heroku create your-app-name
heroku config:set DB_HOST=your-db-host
heroku config:set DB_USER=your-db-user
heroku config:set DB_PASSWORD=your-db-password
heroku config:set DB_NAME=game_rating
heroku config:set JWT_SECRET=your-secret-key
git push heroku main
```

После деплоя получите URL бэкенда (например: `https://your-app.herokuapp.com`)

### 3. Обновите .env.production

В файле `.env.production` укажите реальный URL бэкенда:

```env
REACT_APP_API_URL=https://your-app.herokuapp.com/api
```

### 4. Сборка и деплой фронтенда

```bash
cd kursach-react

# Установите зависимости (если еще не установлены)
npm install

# Соберите проект для продакшена
npm run build

# Деплой на Firebase
firebase deploy
```

### 5. Проверка firebase.json

Убедитесь, что `firebase.json` настроен правильно:

```json
{
  "hosting": {
    "public": "build",
    "rewrites": [
      {
        "source": "**",
        "destination": "/index.html"
      }
    ]
  }
}
```

## Альтернативное решение: Использовать Firebase Functions

Если не хотите деплоить отдельный бэкенд, можно использовать Firebase Functions для API.

## Проверка после деплоя

1. Откройте консоль браузера (F12)
2. Проверьте ошибки сети
3. Убедитесь, что запросы идут на правильный URL бэкенда
4. Проверьте CORS настройки на бэкенде

## Частые ошибки

### CORS Error
- Убедитесь, что на бэкенде разрешены запросы с вашего Firebase домена
- Проверьте настройки CORS в `server/server.js`

### 404 Not Found
- Проверьте, что `firebase.json` указывает на папку `build`
- Убедитесь, что выполнили `npm run build`

### API не отвечает
- Проверьте, что бэкенд запущен и доступен
- Проверьте URL в `.env.production`
- Проверьте логи бэкенда

