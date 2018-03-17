#ifndef __NOCOPYABLE_H_
#define __NOCOPYABLE_H_

namespace jpp {
	class nocopyable {
	protected:
		nocopyable() {};
		virtual ~nocopyable() {};

	private:
		nocopyable(const nocopyable &);
		nocopyable & operator= (const nocopyable &);
	};
}

#endif
