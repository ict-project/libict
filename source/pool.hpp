//! @file
//! @brief Pool module - Header file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2012-2017
//! @copyright ICT-Project Mariusz Ornowski (ict-project.pl)
//! 
//! Pula pozwala na ponowne wykorzystanie (w pewnych ustalonych konfiguracyjnie ramach)
//! danego typu (T) zasobów. Zarządzane przez pulę zasoby muszą spełniać następujące warunki:
//! @li Zasób w każdej chwili może być skasowany (pula sama kasuje obiekty w razie potrzeby).
//! Konfiguracja puli opiera się na zdefiniowaniu następujących jej parametrów:
//! @li Maksymalny rozmiar puli (jeśli zero, to bez ograniczeń);
//! @li Minimalny rozmiar puli (jeśli zero, to bez ograniczeń);
//! @li Maksymalna liczba użyć elementu puli (jeśli zero, to bez ograniczeń);
//! @li Maksymalny czas życia elementu puli (jeśli zero, to bez ograniczeń);
//! @li Maksymalny czas oczekiwania elementu puli na użycie (jeśli zero, to bez ograniczeń).
//! Pula używa std::shared_ptr do udostępniania elementów (zasobów).
//!
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
#ifndef _POOL_HEADER
#define _POOL_HEADER
//============================================
#include "time.hpp"
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <mutex>
//============================================
namespace ict { namespace pool {
//============================================
template<typename T> 
class Distributor;

template<typename T>
class ItemPtr: public std::shared_ptr<T> {
private:
  void callDistributor() noexcept {
    try {
      if (std::shared_ptr<T>::use_count()>1) 
        if (std::shared_ptr<T>::get()) 
          std::shared_ptr<T>::get()->callDistributor();
    } catch (...) {
    }
  }
public:
  ItemPtr() noexcept : std::shared_ptr<T>(){}
  ItemPtr(const ItemPtr & p) noexcept : std::shared_ptr<T>(p){}
  ItemPtr(const std::shared_ptr<T> & p) noexcept : std::shared_ptr<T>(p){}
  ItemPtr(T * p) noexcept : std::shared_ptr<T>(p){}
  ~ItemPtr(){
    callDistributor();
  }
  void reset(T* p){
    std::shared_ptr<T>::reset(p);
    callDistributor();
  }
};

template<typename T>
class Item{
  friend ItemPtr<T>;
  friend Distributor<T>;
  Distributor<T> * distributor=nullptr;
protected:
  void callDistributor(){
    if (distributor)  distributor->callDistributor();
  }
  void setDistributor(Distributor<T> * d){
    distributor=d;
  }
  void clearDistributor(){
    distributor=nullptr;
  }
};

class DistributorBase{
private:
  //mutable std::mutex mutex;
  uint32_t max_size=0;
  uint32_t min_size=0;
  uint32_t max_use_count=0;
  uint32_t max_life_time=0;
  uint32_t max_idle_time=0;
  struct item_t {
    //! Czas utworzenia elementu puli.
    ict::time::TimeInfo creation_time;
    //! Czas ostatniego użycia elementu puli.
    ict::time::TimeInfo last_use_time;
    //! Licznik użycia elementu puli.
    uint32_t use_count=0;
  };
  typedef std::map<std::size_t,item_t> item_list_t;
  //! Lista elementów puli.
  item_list_t item_list;
  //! Zwraca nowy indeks elementu.
  std::size_t getNewIndex();
  //! Tworzy nowy element.
  bool createItem(std::size_t k);
  //! Usuwa nowy element.
  bool deleteItem(std::size_t k);
  //! Sprawdza, czy wskazany element można użyć.
  bool testItem(std::size_t k);
  //! Zwiększa licznik użycia.
  bool useItem(std::size_t k);
protected:
  DistributorBase(uint32_t maxSize=0,uint32_t minSize=0,uint32_t maxUseCount=0,uint32_t maxLifeTime=0,uint32_t maxIdleTime=0);
  ~DistributorBase();
  //!
  //! Tworzy nowy element puli.
  //!
  //! @param k Indeks elementu.
  //! @return Sukces lub porażka.
  //!
  virtual bool itemCreate(std::size_t k)=0;
  //!
  //! Usuwa element puli.
  //!
  //! @param k Indeks elementu.
  //! @return Sukces lub porażka.
  //!
  virtual bool itemDelete(std::size_t k)=0;
  //!
  //! Zwraca informację, czy element puli jest dostępny i gotowy do użycia.
  //!
  //! @param k Indeks elementu.
  //! @return Dostępny- 'true', niedostępny - 'false'.
  //!
  virtual bool itemReady(std::size_t k)=0;
  //!
  //! Zwraca indeks elementu, który jest dostępny i gotowy do użycia.
  //!
  //! @return Indeks elementu lub -1, gdy żaden element nie jest dostępny.
  //!
  std::size_t selectItem();
public:
  //!
  //! @brief Ustawia maksymalny rozmiar puli (jeśli zero, to bez ograniczeń).
  //! 
  //! @param value Maksymalny rozmiar.
  //!
  void maxSize(uint32_t value=0);
  //!
  //! @brief Ustawia minimalny rozmiar puli (jeśli zero, to bez ograniczeń).
  //! 
  //! @param value Minimalny rozmiar.
  //!
  void minSize(uint32_t value=0);
  //!
  //! @brief Ustawia maksymalną liczbę użyć elementu puli (jeśli zero, to bez ograniczeń).
  //! 
  //! @param value Liczba użyć.
  //!
  void maxUseCount(uint32_t value=0);
  //!
  //! @brief Ustawia maksymalny czas życia elementu puli (jeśli zero, to bez ograniczeń).
  //! 
  //! @param value Czas w sekundach.
  //!
  void maxLifeTime(uint32_t value=0);
  //!
  //! @brief Ustawia maksymalny czas oczekiwania elementu puli na użycie (jeśli zero, to bez ograniczeń).
  //! 
  //! @param value Czas w sekundach.
  //!
  void maxIdleTime(uint32_t value=0);
  //!
  //! Przegląda elementy puli i kasuje zbędne.
  //!
  void clean();
  //!
  //! Kasuje wszystkie elementy puli.
  //!
  void clear();
  //!
  //! Zwraca aktualny rozmiar puli.
  //!
  std::size_t size();
};

template<typename T> 
class Distributor: public DistributorBase {
public:
  typedef std::function<T*(void)> item_creator_t;
  typedef std::function<void(ItemPtr<T> & item)> job_t;
private:
  typedef std::map<std::size_t,ItemPtr<T>> item_list_t;
  typedef std::queue<job_t> job_queue_t;
  //mutable std::mutex mutex;
  item_creator_t item_creator;
  item_list_t item_list;
  job_queue_t job_queue;
  bool itemCreate(std::size_t k){
    if (item_list.count(k)){
      return(false);
    } else {
      T * i=item_creator();
      if (!i) return(false);
      item_list.emplace(k,i);
    }
    return(true);
  }
  bool itemDelete(std::size_t k){
    if (item_list.count(k)){
      item_list.erase(k);
      return(true);
    }
    return(false);
  }
  bool itemReady(std::size_t k){
    if (item_list.count(k)){
      return(item_list.at(k).unique());
    }
    return(false);
  }
public:
  Distributor(item_creator_t creator,uint32_t maxSize=0,uint32_t minSize=0,uint32_t maxUseCount=0,uint32_t maxLifeTime=0,uint32_t maxIdleTime=0):
    DistributorBase(maxSize,minSize,maxUseCount,maxLifeTime,maxIdleTime),item_creator(creator){}
  ~Distributor(){
    //std::unique_lock<std::mutex> lock(mutex);
    ItemPtr<T> item;
    while(!job_queue.empty()){
      job_queue.front()(item);
      job_queue.pop();
    }
    for(typename item_list_t::iterator it=item_list.begin();it!=item_list.end();++it){
      it->second->clearDistributor();
    }
  }
  void callDistributor(){
    ItemPtr<T> item;
    job_t job;
    {
      //std::unique_lock<std::mutex> lock(mutex);
      if (!job_queue.empty()){
        std::size_t k=selectItem();
        if (item_list.count(k)){
          item=item_list[k];
          job=job_queue.front();
          job_queue.pop();
        }
      }
    }
    if (item) item->setDistributor(this);
    if (job) job(item);
  }
  void addJob(job_t job){
    ItemPtr<T> item;
    {
      //std::unique_lock<std::mutex> lock(mutex);
      std::size_t k=selectItem();
      if (item_list.count(k)){
        item=item_list[k];
      } else {
        job_queue.push(job);
        return;
      }
    }
    if (item) item->setDistributor(this);
    if (job) job(item);
  }
};
//============================================
}}
//============================================
#endif
//============================================