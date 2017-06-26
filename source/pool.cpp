//! @file
//! @brief Pool module - Source file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2012-2017
//! @copyright ICT-Project Mariusz Ornowski (ict-project.pl)
/* **************************************************************
Copyright (c) 2012-2017, ICT-Project Mariusz Ornowski (ict-project.pl)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the ICT-Project Mariusz Ornowski nor the names
of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************/
//============================================
#include "pool.hpp"
#include "logger.hpp"
#include <set>
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace pool {
//============================================
DistributorBase::DistributorBase(uint32_t maxSize,uint32_t minSize,uint32_t maxUseCount,uint32_t maxLifeTime,uint32_t maxIdleTime):
  max_size(maxSize),min_size(minSize),max_use_count(maxUseCount),max_life_time(maxLifeTime),max_idle_time(maxIdleTime)
{
  LOGGER_INFO<<__LOGGER__<<"Utworzono pulę (p="<<this<<"): ";
  LOGGER_INFO<<"max_size="<<maxSize<<",min_size="<<minSize<<",max_use_count="<<maxUseCount<<",max_life_time="<<maxLifeTime<<",max_idle_time="<<maxIdleTime;
  LOGGER_INFO<<std::endl;
}
DistributorBase::~DistributorBase(){
  LOGGER_INFO<<__LOGGER__<<"Usunięto pulę (p="<<this<<")."<<std::endl;
}
std::size_t DistributorBase::getNewIndex(){
  if (item_list.size()){
    std::size_t k=0;
    for (item_list_t::const_iterator it=item_list.cbegin();it!=item_list.cend();++it){
      if (k!=(it->first)) return(k);
      k++;
    }
    return(k);
  } else {
    return(0);
  }
}
bool DistributorBase::createItem(std::size_t k){
  if (item_list.count(k)){
    LOGGER_ERR<<__LOGGER__<<"Błąd utworzenia elementu puli (k="<<k<<",p="<<this<<")!"<<std::endl;
    return(false);
  }
  if (itemCreate(k)){
    item_list[k];
    LOGGER_DEBUG<<__LOGGER__<<"Utworzono element puli (k="<<k<<",p="<<this<<")!"<<std::endl;
    return(true);
  }
  LOGGER_ERR<<__LOGGER__<<"Błąd utworzenia elementu puli (k="<<k<<",p="<<this<<")!"<<std::endl;
  return(false);
}
bool DistributorBase::deleteItem(std::size_t k){
  if (!item_list.count(k)){
    LOGGER_ERR<<__LOGGER__<<"Błąd usuwania elementu puli (k="<<k<<",p="<<this<<")!"<<std::endl;
    return(false);
  }
  item_list.erase(k);
  if (itemDelete(k)){
    LOGGER_DEBUG<<__LOGGER__<<"Usunięto element puli (k="<<k<<",p="<<this<<")!"<<std::endl;
    return(true);
  }
  LOGGER_ERR<<__LOGGER__<<"Błąd usuwania elementu puli (k="<<k<<",p="<<this<<")!"<<std::endl;
  return(false);
}
bool DistributorBase::testItem(std::size_t k){
  if (item_list.count(k)){
    const item_t & item(item_list.at(k));
    ict::time::TimeInfo now;
    if (max_life_time) if ((item.creation_time.unix_sec+max_life_time)<now.unix_sec) return(false);
    if (max_idle_time) if ((item.last_use_time.unix_sec+max_idle_time)<now.unix_sec) return(false);
    if (max_use_count) if (max_use_count<item.use_count) return(false);
    return(true);
  }
  LOGGER_ERR<<__LOGGER__<<"Błąd testowania elementu puli (k="<<k<<")!"<<std::endl;
  return(false);
}
bool DistributorBase::useItem(std::size_t k){
  if (item_list.count(k)){
    item_t & item(item_list[k]);
    item_list[k].last_use_time.setLocalTime();
    item_list[k].use_count++;
    return(true);
  }
  LOGGER_ERR<<__LOGGER__<<"Błąd użycia elementu puli (k="<<k<<")!"<<std::endl;
  return(false);
}
std::size_t DistributorBase::selectItem(){
  std::set<std::size_t> deleteList;// Lista elementów do usunięcia.
  std::size_t s=item_list.size();
  if (s<min_size){//Jeśli rozmiar puli mniejszy od wymaganego, to tworzę nowy element.
    std::size_t k=getNewIndex();
    if (createItem(k)){
      useItem(k);
      return(k);
    }
  }
  for (item_list_t::const_iterator it=item_list.cbegin();it!=item_list.cend();++it){//Wyszukiwanie wolnego elementu.
    if (testItem(it->first)){
      if (itemReady(it->first)){
        useItem(it->first);
        return(it->first);
      }
    } else {
      deleteList.insert(it->first);
    }
  }
  for (auto & item : deleteList) deleteItem(item);//Kasuję zbedne elementy.
  if ((max_size==0)||(s<max_size)){//Jeśli rozmiar puli mniejszy od dozwolonego, to tworzę nowy element.
    std::size_t k=getNewIndex();
    if (createItem(k)){
      useItem(k);
      return(k);
    }
  }
  return(-1);
}
void DistributorBase::maxSize(uint32_t value){
  std::unique_lock<std::mutex> lock(mutex);
  max_size=value;
  if (value) if (min_size>max_size) min_size=max_size;
}
void DistributorBase::minSize(uint32_t value){
  std::unique_lock<std::mutex> lock(mutex);
  min_size=value;
  if (value) if (max_size<min_size) max_size=min_size;
}
void DistributorBase::maxUseCount(uint32_t value){
  std::unique_lock<std::mutex> lock(mutex);
  max_use_count=value;
}
void DistributorBase::maxLifeTime(uint32_t value){
  std::unique_lock<std::mutex> lock(mutex);
  max_life_time=value;
}
void DistributorBase::maxIdleTime(uint32_t value){
  std::unique_lock<std::mutex> lock(mutex);
  max_idle_time=value;
}
void DistributorBase::clean(){
  std::unique_lock<std::mutex> lock(mutex);
  std::set<std::size_t> deleteList;// Lista elementów do usunięcia.
  for (item_list_t::const_iterator it=item_list.cbegin();it!=item_list.cend();++it){//Wyszukiwanie.
    if (!testItem(it->first)){
      deleteList.insert(it->first);
    }
  }
  for (auto & item : deleteList) deleteItem(item);//Kasuję zbedne elementy.
}
std::size_t DistributorBase::size(){
  std::unique_lock<std::mutex> lock(mutex);
  return(item_list.size());
}
//============================================
}}
//============================================
#ifdef ENABLE_TESTING
class TestItem : public ict::pool::Item<TestItem>{
public:
  void doPing(){
    std::cout<<"Ping! ";
  }
};
REGISTER_TEST(pool,tc1){
  ict::pool::Distributor<TestItem> pool([](){
    return(new TestItem);
  });
  std::cout<<" Test puli ict::pool::Distributor(c)"<<std::endl;
  for (int k=0;k<20;k++) {
    pool.addJob([](ict::pool::ItemPtr<TestItem> & item){
      item->doPing();
    });
    if (pool.size()!=1){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" size="<<pool.size()<<std::endl;
      std::cout<<" k="<<k<<std::endl;
      return(-1);
    }
  }
  std::cout<<std::endl;
  return(0);
}
REGISTER_TEST(pool,tc2){
  {
    std::set<ict::pool::ItemPtr<TestItem>> items;
    ict::pool::Distributor<TestItem> pool([](){
      return(new TestItem);
    });
    std::cout<<" Test puli ict::pool::Distributor(c)"<<std::endl;
    std::cout<<"Przed:"<<std::endl;
    for (int k=0;k<20;k++) {
      pool.addJob([&](ict::pool::ItemPtr<TestItem> & item){
        items.insert(item);
        item->doPing();
      });
      if (pool.size()!=(k+1)){
        std::cout<<" Błąd!!!"<<std::endl;
        std::cout<<" size="<<pool.size()<<std::endl;
        std::cout<<" k="<<k<<std::endl;
        return(-1);
      }
    }
    std::cout<<std::endl<<"Po:"<<std::endl;
    items.clear();
  }
  std::cout<<std::endl;
  return(0);
}
REGISTER_TEST(pool,tc3){
  {
    std::set<ict::pool::ItemPtr<TestItem>> items;
    ict::pool::Distributor<TestItem> pool([](){
      return(new TestItem);
    },5);
    std::cout<<" Test puli ict::pool::Distributor(c,5)"<<std::endl;
    std::cout<<"Przed:"<<std::endl;
    for (int k=0;k<20;k++) {
      pool.addJob([&](ict::pool::ItemPtr<TestItem> & item){
        items.insert(item);
        item->doPing();
      });
      if (k<5){
        if (pool.size()!=(k+1)){
          std::cout<<" Błąd!!!"<<std::endl;
          std::cout<<" size="<<pool.size()<<std::endl;
          std::cout<<" k="<<k<<std::endl;
          return(-1);
        }
      } else {
        if (pool.size()!=5){
          std::cout<<" Błąd!!!"<<std::endl;
          std::cout<<" size="<<pool.size()<<std::endl;
          std::cout<<" k="<<k<<std::endl;
          return(-1);
        }
      }
    }
    std::cout<<std::endl<<"Po:"<<std::endl;
    items.clear();
  }
  std::cout<<std::endl;
  return(0);
}
REGISTER_TEST(pool,tc4){
  ict::pool::Distributor<TestItem> pool([](){
    return(new TestItem);
  },0,5);
  std::cout<<" Test puli ict::pool::Distributor(c,0,5)"<<std::endl;
  for (int k=0;k<20;k++) {
    pool.addJob([](ict::pool::ItemPtr<TestItem> & item){
      item->doPing();
    });
    if (k<5){
      if (pool.size()!=(k+1)){
        std::cout<<" Błąd!!!"<<std::endl;
        std::cout<<" size="<<pool.size()<<std::endl;
        std::cout<<" k="<<k<<std::endl;
        return(-1);
      }
    } else {
      if (pool.size()!=5){
        std::cout<<" Błąd!!!"<<std::endl;
        std::cout<<" size="<<pool.size()<<std::endl;
        std::cout<<" k="<<k<<std::endl;
        return(-1);
      }
    }
  }
  std::cout<<std::endl;
  return(0);
}
#endif
//===========================================