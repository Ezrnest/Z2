/*
 * Created by liyicheng on 2019/7/9.
 */

#ifndef Z2_SERIALIZABLE_H
#define Z2_SERIALIZABLE_H

#include <ostream>
#include <functional>
#include <iostream>
using namespace std;
namespace z2 {
/**
 * <h2>Serializing</h2>
 * When serializing, the serializable object should firstly save its class name.
 * <h2>Deserializing</h2>
 */
class Serializable {
public:
    /**
     * Serialize `this` to the output stream.
     * @param output
     */
    virtual void serializeTo(ostream &output) = 0;


    /**
     * The method that will create a new object of this type. There is no leading class name.
     * <br> <b>This method is only an example!</b>
     * <br><b>Subclasses of this interface should define a static method the same as this method.</b>
     */
    static Serializable* loadFrom(istream& input);

    /**
     * Gets the class name.
     */
    virtual const string& getClassName() const = 0;

public:
    template<typename C>
    static void serializeCollection(C coll, ostream &output);

    template<typename T,typename C>
    static C deserializeCollection(function<C(int)> provider, istream &input);

    template<typename T,typename C>
    static void deserializeCollection(C& coll, istream &input);
};


template<typename C>
void z2::Serializable::serializeCollection(C coll, ostream &output) {
    output << coll.size() << ' ';
    for(auto& it : coll){
        output << it << ' ';
    }
}

template<typename T,typename C>
C z2::Serializable::deserializeCollection(function<C(int)> provider, istream &input){
    int size;
    input >> size;
    C coll = provider(size);
    for (int i = 0; i < size; ++i) {
        T t;
        input >> t;
        coll.insert(t);
    }
    return coll;
}

template<typename T, typename C>
void z2::Serializable::deserializeCollection(C &coll, istream &input) {
    int size;
    input >> size;
    for (int i = 0; i < size; ++i) {
        T t;
        input >> t;
        coll.insert(t);
    }
}

class DataSerializable : public Serializable{
public:
    void serializeTo(ostream &output) override;

protected:
    virtual void serializeData(ostream &output) = 0 ;
public:
    virtual void deserializeData(istream &input) = 0;
};

}




#endif //Z2_SERIALIZABLE_H
