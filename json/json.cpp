#include <iostream>
using namespace std;

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

void Serialize_1()
{
    rapidjson::StringBuffer strBuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);

    writer.StartObject();

    //1. 整数类型
    writer.Key("Int");
    writer.Int(1);

    //2. 浮点类型
    writer.Key("Double");
    writer.Double(12.0000001);

    //3. 字符串类型
    writer.Key("String");
    writer.String("This is a string");

    //4. 结构体类型
    writer.Key("Object");
    writer.StartObject();
    writer.Key("name");
    writer.String("qq849635649");
    writer.Key("age");
    writer.Int(25);
    writer.EndObject();

    //5. 数组类型
    //5.1 整型数组
    writer.Key("IntArray");
    writer.StartArray();
    //顺序写入即可
    writer.Int(10);
    writer.Int(20);
    writer.Int(30);
    writer.EndArray();

    //5.2 浮点型数组
    writer.Key("DoubleArray");
    writer.StartArray();
    for(int i = 1; i < 4; i++)
    {
        writer.Double(i * 1.0);
    }
    writer.EndArray();

    //5.3 字符串数组
    writer.Key("StringArray");
    writer.StartArray();
    writer.String("one");
    writer.String("two");
    writer.String("three");
    writer.EndArray();

    //5.4 结构体数组
    writer.Key("People");
    writer.StartArray();
    for(int i = 0; i < 3; i++)
    {
        writer.StartObject();
        writer.Key("name");
        writer.String("qq849635649");
        writer.Key("age");
        writer.Int(i * 10);
        writer.Key("sex");
        writer.Bool((i % 2) == 0);
        writer.EndObject();
    }
    writer.EndArray();

    writer.EndObject();

    string data = strBuf.GetString();
    cout << data << endl;
}

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

void Serialize_2()
{
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    //1. 整型类型
    doc.AddMember("Int", 1, allocator);

    //2. 浮点类型
    doc.AddMember("Double", 12.00001, allocator);

    //3. 字符串类型
    //正确方式
    string str= "This is a string";
    rapidjson::Value str_value(rapidjson::kStringType);
    str_value.SetString(str.c_str(), str.size());
    if(!str_value.IsNull())
    {
        doc.AddMember("String", str_value, allocator);
    }
    /**
     *    注：以下方式不正确，可能成功，也可能失败，因为字符串写入json要重新开辟内存，
     * 如果使用该方式的话，当数据是字符串常量的话是没问题的，如果为变量就会显示乱码，所
     * 以为保险起见，我们显式的分配内存(无需释放)
     */
    //doc.AddMember("String", str.data(), allocator);

    //4. 结构体类型
    rapidjson::Value object(rapidjson::kObjectType);
    object.AddMember("name", "qq849635649", allocator); //注：常量是没有问题的
    object.AddMember("age", 25, allocator);
    doc.AddMember("Object", object, allocator);

    //5. 数组类型
    //5.1 整型数组
    rapidjson::Value IntArray(rapidjson::kArrayType);
    IntArray.PushBack(10, allocator);
    IntArray.PushBack(20, allocator);
    IntArray.PushBack(30, allocator);
    doc.AddMember("IntArray", IntArray, allocator);

    //5.2 浮点型数组
    rapidjson::Value DoubleArray(rapidjson::kArrayType);
    DoubleArray.PushBack(1.0, allocator);
    DoubleArray.PushBack(2.0, allocator);
    DoubleArray.PushBack(3.0, allocator);
    doc.AddMember("DoubleArray", DoubleArray, allocator);

    //5.3 字符型数组
    rapidjson::Value StringArray(rapidjson::kArrayType);
    string strValue1 = "one";
    string strValue2 = "two";
    string strValue3 = "three";
    str_value.SetString(strValue1.c_str(), strValue1.size());
    StringArray.PushBack(str_value, allocator);
    str_value.SetString(strValue2.c_str(), strValue2.size());
    StringArray.PushBack(str_value, allocator);
    str_value.SetString(strValue3.c_str(), strValue3.size());
    StringArray.PushBack(str_value, allocator);
    doc.AddMember("StringArray", StringArray, allocator);

    //5.4 结构体数组
    rapidjson::Value ObjectArray(rapidjson::kArrayType);
    for(int i = 1; i < 4; i++)
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("name", "qq849635649", allocator);//注：常量是没有问题的
        obj.AddMember("age", i * 10, allocator);
        ObjectArray.PushBack(obj, allocator);
    }
    doc.AddMember("ObjectArray", ObjectArray, allocator);

    rapidjson::StringBuffer strBuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);
    doc.Accept(writer);

    string data = strBuf.GetString();
    cout << data << endl;
}

void parse() {
    string data  ="{\"Int\":1,\"Double\":12.00001,\"String\":\"This is a string\",\"Object\":{\"name\":\"qq849635649\",\"age\":25},\"IntArray\":[10,20,30],\"DoubleArray\":[1.0,2.0,3.0],\"StringArray\":[\"one\",\"two\",\"three\"],\"ObjectArray\":[{\"name\":\"qq849635649\",\"age\":10},{\"name\":\"qq849635649\",\"age\":20},{\"name\":\"qq849635649\",\"age\":30}]}";

    //创建解析对象
    rapidjson::Document doc;
    //首先进行解析，没有解析错误才能进行具体字段的解析
    if(!doc.Parse(data.data()).HasParseError())
    {
        //1. 解析整数
        if(doc.HasMember("Int") && doc["Int"].IsInt())
        {
            cout << "Int = " << doc["Int"].GetInt() << endl;
        }
        //2. 解析浮点型
        if(doc.HasMember("Double") && doc["Double"].IsDouble())
        {
            cout << "Double = " << doc["Double"].GetDouble() << endl;
        }
        //3. 解析字符串
        if(doc.HasMember("String") && doc["String"].IsString())
        {
            cout << "String = " << doc["String"].GetString() << endl;
        }
        //4. 解析结构体
        if(doc.HasMember("Object") && doc["Object"].IsObject())
        {
            const rapidjson::Value& object = doc["Object"];
            if(object.HasMember("name") && object["name"].IsString())
            {
                cout << "Object.name = " << object["name"].GetString() << endl;
            }
            if(object.HasMember("age") && object["age"].IsInt())
            {
                cout << "Object.age = " << object["age"].GetInt() << endl;
            }
        }
        //5. 解析数组类型
        //5.1 整型数组类型
        if(doc.HasMember("IntArray") && doc["IntArray"].IsArray())
        {
            //5.1.1 将字段转换成为rapidjson::Value类型
            const rapidjson::Value& array = doc["IntArray"];
            //5.1.2 获取数组长度
            size_t len = array.Size();
            //5.1.3 根据下标遍历，注意将元素转换为相应类型，即需要调用GetInt()
            for(size_t i = 0; i < len; i++)
            {
                cout << "IntArray[" << i << "] = " << array[i].GetInt() << endl;
            }
        }
        //5.2 浮点型数组类型
        if(doc.HasMember("DoubleArray") && doc["DoubleArray"].IsArray())
        {
            const rapidjson::Value& array = doc["DoubleArray"];
            size_t len = array.Size();
            for(size_t i = 0; i < len; i++)
            {
                //为防止类型不匹配，一般会添加类型校验
                if(array[i].IsDouble())
                {
                    cout << "DoubleArray[" << i << "] = " << array[i].GetDouble() << endl;
                }
            }
        }
        //5.3 字符串数组类型
        if(doc.HasMember("StringArray") && doc["StringArray"].IsArray())
        {
            const rapidjson::Value& array = doc["StringArray"];
            size_t len = array.Size();
            for(size_t i = 0; i < len; i++)
            {
                //为防止类型不匹配，一般会添加类型校验
                if(array[i].IsString())
                {
                    cout << "StringArray[" << i << "] = " << array[i].GetString() << endl;
                }
            }
        }
        //5.4 结构体数组类型
        if(doc.HasMember("ObjectArray") && doc["ObjectArray"].IsArray())
        {
            const rapidjson::Value& array = doc["ObjectArray"];
            size_t len = array.Size();
            for(size_t i = 0; i < len; i++)
            {
                const rapidjson::Value& object = array[i];
                //为防止类型不匹配，一般会添加类型校验
                if(object.IsObject())
                {
                    if(object.HasMember("name") && object["name"].IsString())
                    {
                        cout << "ObjectArray[" << i << "].name = "
                             << object["name"].GetString() << endl;
                    }
                    if(object.HasMember("age") && object["age"].IsInt())
                    {
                        cout << "ObjectArray[" << i << "].age = "
                             << object["age"].GetInt() << endl;
                    }
                }
            }
        }
    }
    /**
     *    最后注意：因为rapidjson不会做安全校验，所以要自己做安全校验，以int整型为例
     * “if(object.HasMember("age") && object["age"].IsInt()) {}”
     * 这句校验很重要，既要校验有该子段，也要校验类型正确，否则会引发程序崩溃
     */
}

int main()
{
    parse();
	return 0;
}
