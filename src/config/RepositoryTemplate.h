/*
 * Created by liyicheng on 2019/8/7.
 */

#ifndef Z2_REPOSITORYTEMPLATE_H
#define Z2_REPOSITORYTEMPLATE_H

#include <util/File.h>

namespace z2 {
template <typename Clazz>
class RepositoryTemplate {
public:
    RepositoryTemplate();

    RepositoryTemplate(const RepositoryTemplate &entityRepository) = delete;

    RepositoryTemplate(RepositoryTemplate &&entityRepository) noexcept = delete;

    RepositoryTemplate &operator=(const RepositoryTemplate &entityRepository) = delete;

    RepositoryTemplate &operator=(RepositoryTemplate &&entityRepository) noexcept = delete;

    virtual ~RepositoryTemplate();

    virtual void initFromFolder(const ancono::File& dir) = 0;
public:
    static Clazz& instance();
};

template<typename Clazz>
Clazz &RepositoryTemplate<Clazz>::instance() {
    static Clazz repo;
    return repo;
}

template<typename Clazz>
RepositoryTemplate<Clazz>::~RepositoryTemplate()= default;
template<typename Clazz>
RepositoryTemplate<Clazz>::RepositoryTemplate()= default;
}

#endif //Z2_REPOSITORYTEMPLATE_H
