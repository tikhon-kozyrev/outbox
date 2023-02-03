/*
	Copyright (c) 2023 Tikhon Kozyrev (tikhon.kozyrev@gmail.com)
*/
#ifndef PATTERNS_OUTBOX_HPP
#define PATTERNS_OUTBOX_HPP

#include <string>
#include <vector>

namespace Patterns {
	template <class KEY, class MESSAGE> class TOutbox {
		public:
			using Key = KEY;
			using Message = MESSAGE;
			struct KeyMessage {
				Key key;
				Message value;
			};
			virtual ~TOutbox()= default;
			virtual bool Open(const std::string &)=0;
			virtual void Close()=0;
			virtual bool Send(const Message &)=0;
			virtual std::vector<KeyMessage> GetNext(size_t)=0;
			virtual bool Done(const Key &)=0;
	};

}

#endif
