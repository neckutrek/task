#include "model.h"
#include "exception.h"

#include <string>
#include <stdexcept>

namespace aspka {

using std::make_shared;
using std::out_of_range;
using std::to_string;

Model::Model() 
: n_portfolios_(0), n_aquirers_(0), n_counterparties_(0), n_marketplaces_(0), 
  n_instruments_(0), n_trades_(0)
{
   addInstrument("ABB", "SEK", "Asea Brown Boveri");
   addInstrument("LME", "SEK", "LM Ericsson");

   addTrade("ABB", "Stock Portfolio", "Equity", "Deutche", "OMX", 14.95, 12, true);
   addTrade("LME", "Stock Portfolio", "Equity", "Deutche", "OMX", 9.55, 15, true);
}

void Model::addInstrument
(const string& name, const string& currency, const string& issuer)
{
   InstrumentMap::iterator it = instrument_map_.begin();
   while (it != instrument_map_.end()) {
      if (name.compare(it->second->name_) == 0) {
         throw Exception("An instrument with name '" + name + "' already exists!");
      }
      ++it;
   }
   
   instrument_map_.emplace(n_instruments_, make_shared<Instrument>(name, currency, issuer));
   ++n_instruments_;
}

shared_ptr<Instrument> Model::getInstrument
(int id)
{
   InstrumentMap::iterator it = instrument_map_.find(id);
   if (it != instrument_map_.end()) {
      return it->second;
   }
   return nullptr;
}

void Model::getAllInstrumentNames
(string& names)
{
   names = "";
   InstrumentMap::iterator it = instrument_map_.begin();
   while (it != instrument_map_.end()) {
      if (it->second != nullptr) {
         names += it->second->name_ + ", ";
      }
      ++it;
   }
   if (names.compare("") != 0) {
      names = names.substr(0, names.length()-2);
   }
}

void Model::addTrade
(const string& name, const string& portfolio, const string& aquirer, 
 const string& counterparty, const string& marketplace, double price, 
 int quantity, bool buy)
{
   int instrument_id = findIdFromInstrumentName(name);
   if (instrument_id < 0) {
      throw Exception("The instrument '" + name + "' was not found!\n");
   }

   int portfolio_id = getIntStringMapId(portfolios_, portfolio);
   if (portfolio_id < 0) {
      portfolio_id = n_portfolios_;
      portfolios_.emplace(portfolio_id, portfolio);
      ++n_portfolios_;
   }

   int aquirer_id = getIntStringMapId(aquirers_, aquirer);
   if (aquirer_id < 0) {
      aquirer_id = n_aquirers_;
      aquirers_.emplace(aquirer_id, aquirer);
      ++n_aquirers_;
   }

   int counterparty_id = getIntStringMapId(counterparties_, counterparty);
   if (counterparty_id < 0) {
      counterparty_id = n_counterparties_;
      counterparties_.emplace(counterparty_id, counterparty);
      ++n_counterparties_;
   }

   int marketplace_id = getIntStringMapId(marketplaces_, marketplace);
   if (marketplace_id < 0) {
      marketplace_id = n_marketplaces_;
      marketplaces_.emplace(marketplace_id, marketplace);
      ++n_marketplaces_;
   }

   trade_map_.emplace(n_trades_, make_shared<Trade>(
      instrument_id, portfolio_id, aquirer_id, counterparty_id, marketplace_id, 
      price, quantity, buy
   ));
   ++n_trades_;
}

shared_ptr<Trade> Model::getTrade
(int id)
{
   TradeMap::iterator it = trade_map_.find(id);
   if (it != trade_map_.end()) {
      return it->second;
   }
   return nullptr;
}

const string& Model::getPortfolioName
(int id)
{
   try { return portfolios_.at(id); } 
   catch (out_of_range& e) {
      throw Exception("ERROR: No portfolio with id '" + to_string(id) + "'!");
   }
}

const string& Model::getAquirerName
(int id)
{
   try { return aquirers_.at(id); } 
   catch (out_of_range& e) {
      throw Exception("ERROR: No aquirer with id '" + to_string(id) + "'!");
   }
}

const string& Model::getCounterpartyName
(int id)
{
   try { return counterparties_.at(id); } 
   catch (out_of_range& e) {
      throw Exception("ERROR: No counterparty with id '" + to_string(id) + "'!");
   }
}

const string& Model::getMarketplaceName
(int id)
{
   try { return marketplaces_.at(id); } 
   catch (out_of_range& e) {
      throw Exception("ERROR: No marketplace with id '" + to_string(id) + "'!");
   }
}





int Model::findIdFromInstrumentName
(const string& name)
{
   int retval = -1;
   InstrumentMap::iterator it = instrument_map_.begin();
   while (retval < 0 && it != instrument_map_.end()) {
      if (it->second != nullptr) {
         if (it->second->name_.compare(name) == 0) {
            retval = it->first;
         }
      }
      ++it;
   }
   return retval;
}

int Model::getIntStringMapId
(const IntStringMap& map, const string& name)
{
   int retval = -1;
   IntStringMap::const_iterator it = map.cbegin();
   while (retval < 0 && it != map.cend()) {
      if (it->second.compare(name) == 0) {
         retval = it->first;
      }
      ++it;
   }
   return retval;
}

} // namespace aspka



