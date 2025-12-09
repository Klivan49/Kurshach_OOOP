# Исправления проблем

## 1. Проблема с подключением к БД

**Ошибка:** `Access denied for user 'root'@'localhost' (using password: NO)`

**Решение:**
1. Убедитесь, что файл `.env` находится в папке `server/`
2. Проверьте, что в `.env` указан правильный пароль:
   ```env
   DB_PASSWORD=49K|ivan1708
   ```
3. Перезапустите сервер после изменения `.env`

## 2. Ошибка 500 при добавлении отзыва

**Причины:**
- Проблема с подключением к БД
- Ошибка при логировании в activitylog

**Исправлено:**
- Добавлена обработка ошибок логирования (не блокирует создание отзыва)
- Улучшено логирование ошибок для отладки

## 3. Добавление картинок для игр

### Шаг 1: Добавить поле в базу данных

Выполните SQL запрос:
```sql
ALTER TABLE game ADD COLUMN ImageURL VARCHAR(500) NULL AFTER Description;
```

Или используйте файл: `server/migrations/add_image_url.sql`

### Шаг 2: Использование картинок

Теперь при создании/обновлении игры можно указать URL картинки:

```json
{
  "Name": "Game Name",
  "ImageURL": "https://example.com/game-image.jpg",
  ...
}
```

### Шаг 3: Где брать картинки?

**Варианты:**
1. **Steam API** - можно получать обложки игр
2. **IGDB API** - база данных игр с картинками
3. **Вручную** - загружать на imgur.com или другой хостинг
4. **Placeholder** - если ImageURL не указан, используется placeholder

### Примеры URL картинок:

```
https://cdn.cloudflare.steamstatic.com/steam/apps/730/header.jpg
https://images.igdb.com/igdb/image/upload/t_cover_big/co1wyy.jpg
```

## 4. Favicon 404

Это не критично, но можно добавить favicon:

1. Поместите `favicon.ico` в папку `public/`
2. Или добавьте в `public/index.html`:
   ```html
   <link rel="icon" href="/favicon.ico" />
   ```

## Проверка после исправлений

1. Перезапустите сервер:
   ```bash
   cd server
   npm start
   ```

2. Проверьте подключение к БД - должно быть:
   ```
   ✅ Database connected successfully
   ```

3. Попробуйте добавить отзыв - должно работать

4. Добавьте картинку для игры через админ-панель или API


