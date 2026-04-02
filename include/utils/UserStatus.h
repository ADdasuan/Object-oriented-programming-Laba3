
#ifndef USERSTATUS_H
#define USERSTATUS_H

/**
 * @file UserStatus.h
 * @brief Перечисление статусов пользователя
 */

/**
 * @enum UserStatus
 * @brief Статусы пользователя форума
 */
enum class UserStatus{
    ONLINE, /// Пользователь онлайн (находится в разделе)
    OFFLINE, /// Пользователь офлайн (не в разделе)
    BANNED /// Пользователь забанен (не может входить в разделы)
};

#endif
