// Created by LuoYu on 2026/6/9.
#pragma once

// 管理器模板
template <typename T>
class Manager {
public:
    static T* instance() {
        static T instance;
        return &instance;
    }

    // 禁用拷贝构造函数
    Manager(const Manager&) = delete;
    // 禁用赋值操作
    Manager& operator=(const Manager&) = delete;

protected:
    Manager() = default;
    ~Manager() = default;
};