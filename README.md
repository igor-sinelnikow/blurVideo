# blurVideo
## Задание
С помощью библиотеки OpenCV реализовать локальное размывание видеопотока.
1. Загружается и показывается видеопоток.
2. При перемещении указателя мыши по изображению происходит размывание (эффект blur) некоторой области вокруг курсора. Когда курсор перемещается, размывание исчезает.
3. Размер области, настройки blur, источник видеопотока должен быть прочитан из ini файла.

## Реализация
Для парсинга ini файла используется [Boost.PropertyTree](https://www.boost.org/doc/libs/1_72_0/doc/html/property_tree.html).
