#pragma once
#include "HashMap.h"
#include "RTTI.h"
#include <gsl/gsl>

namespace FieaGameEngine
{
    /// <summary>
    /// Templated abstract Factory class. Serves two purposes: a manager for all other
    /// factories through its static methods and also an abstract base class for all
    /// factories.
    /// </summary>
    template <typename T>
    class Factory
    {
    public:
        /// <summary>
        /// Pure virtual ClassName method. Each concrete factory must define what
        /// its own class name is and return it through this.
        /// </summary>
        /// <returns> The name of the class this factory constructs </returns>
        virtual const std::string& ClassName() const = 0;

        /// <summary>
        /// Pure virtual Create method. Each concrete factory must define how to 
        /// construct its own class and return a pointer to it through this method.
        /// </summary>
        /// <returns> A pointer to a newly constructed class through the factory </returns>
        virtual gsl::owner<T*> Create() const = 0;

        /// <summary>
        /// Finds a concrete factory based on the passed in name
        /// </summary>
        /// <param name="className"> The name of the class to find the factory for </param>
        /// <returns> A pointer to the found factory, nullptr if the factory couldn't
        /// be found </returns>
        static const Factory* const Find(const std::string& className);

        /// <summary>
        /// Creates a class through a concrete factory based on the name passed in
        /// </summary>
        /// <param name="className"> The name of the class to construct </param>
        /// <returns> A pointer to the newly heap allocated class </returns>
        /// <exception cref="std::runtime_error"> If the class name passed in does not have a
        /// concrete factory registered with this factory manager </exception>
        static gsl::owner<T*> Create(const std::string& className);

        /// <summary>
        /// Returns the number of registered concrete factories
        /// </summary>
        /// <returns> The number of registered concrete factories </returns>
        static size_t Size();

        /// <summary>
        /// If this factory manager has any concrete factories registered 
        /// </summary>
        /// <returns> True if there are no conrete factories registered, false
        /// otherwise </returns>
        static bool IsEmpty();
        
    protected:

        /// <summary>
        /// Registers a class with the factory manager and adds it to its internal hashmap
        /// </summary>
        /// <param name="factory"> The factory to register </param>
        /// <exception cref="std::runtime_error"> If there is already a concrete factory registered
        /// with the same class name </exception>
        static void Add(const Factory& factory);

        /// <summary>
        /// Attempts to remove a concrete factory from the internal hashmap of registered factories.
        /// If the factory isn't in the hashmap, nothing happens.
        /// </summary>
        /// <param name="factory"> The factory to remove </param>
        static void Remove(const Factory& factory);

    private:
        /// <summary>
        /// Internal hashmap to keep track of registered factories
        /// </summary>
        inline static HashMap<std::string, const Factory* const> m_factories;
    };

    /// <summary>
    /// Concrete Factory macro that assists in quickly creating factory classes
    /// </summary>
    #define ConcreteFactory(ConcreteT, AbstractT)                                                           \
    class ConcreteT##Factory final : public FieaGameEngine::Factory<AbstractT>                                 \
    {                                                                                                   \
    public:                                                                                             \
        ConcreteT##Factory() { Add(*this); }                                                            \
        ~ConcreteT##Factory() { Remove(*this); }                                                        \
        const std::string & ClassName() const override { return m_classname; }                          \
        gsl::owner<AbstractT*> Create() const override { return new ConcreteT(); }                      \
    private:                                                                                            \
        inline static const std::string m_classname { #ConcreteT };                                     \
    };        

}

#include "Factory.inl"