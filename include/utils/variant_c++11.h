// Project loggerpp
//
// MIT License
//
// Copyright (C) 2018 Dmitry Shatilov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Original sources:
//   https://github.com/shatilov-diman/loggerpp/
//   https://bitbucket.org/charivariltd/loggerpp/
//
// Author contacts:
//   Dmitry Shatilov (e-mail: shatilov.diman@gmail.com; site: https://www.linkedin.com/in/shatilov)
//
//

#pragma once

#include "../utils/noncopyable.h"

#include "../utils/bool_t.h"
#include "optional.h"

#include <type_traits>

namespace charivari_ltd
{
namespace utils
{
	template <typename arg_t, typename ... args_t>
	class variant :
		variant<args_t...>
	{
		optional<arg_t> _value;

	public:
		variant() = default;

		template <typename type_t, typename = std::enable_if_t<std::is_convertible<type_t, arg_t>::value>>
		variant(const type_t& arg)
		{
			assign(arg);
		}
		template <typename type_t, std::enable_if_t< ! std::is_convertible<type_t, arg_t>::value, int> = 0>
		variant(const type_t& arg) :
			variant<args_t...>(arg)
		{}

		template <typename type_t, typename = std::enable_if_t<std::is_convertible<type_t, arg_t>::value>>
		variant& operator=(const type_t& arg)
		{
			assign(arg);
			return *this;
		}
		template <typename type_t, std::enable_if_t< ! std::is_convertible<type_t, arg_t>::value, int> = 0>
		variant& operator=(const type_t& arg)
		{
			variant<args_t...>::operator=(arg);
			return *this;
		}

		template <typename value_t, typename = std::enable_if_t<std::is_same<value_t, arg_t>::value>>
		value_t get() const
		{
			if (_value)
				return *_value;
			throw std::runtime_error("bad_variant_access");
		}

		template <typename value_t, std::enable_if_t< ! std::is_same<value_t, arg_t>::value, int> = 0>
		value_t get() const
		{
			return variant<args_t...>::template get<value_t>();
		}

		template <typename callable_t>
		auto visit(const callable_t& cb) const
		{
			if (_value)
				return cb(*_value);
			return variant<args_t...>::visit(cb);
		}

	private:
		template <typename type_t>
		void assign(const type_t& arg)
		{
			if (assign_same(arg))
				return;
			if (assign_convertible(arg))
				return;
			if (assign_assignable(arg))
				return;
			throw std::runtime_error("bad_variant_assign");
		}

	protected:
		template <typename type_t, typename = std::enable_if_t<std::is_same<type_t, arg_t>::value>>
		bool assign_same(const type_t& arg)
		{
			_value = arg;
			return true;
		}
		template <typename type_t, std::enable_if_t< ! std::is_same<type_t, arg_t>::value, int> = 0>
		bool assign_same(const type_t& arg)
		{
			return variant<args_t...>::assign_same(arg);
		}

	protected:
		template <typename type_t, typename = std::enable_if_t<std::is_convertible<type_t, arg_t>::value>>
		bool assign_convertible(const type_t& arg)
		{
			_value = static_cast<arg_t>(arg);
			return true;
		}
		template <typename type_t, std::enable_if_t< ! std::is_convertible<type_t, arg_t>::value, int> = 0>
		bool assign_convertible(const type_t& arg)
		{
			return variant<args_t...>::assign_same(arg);
		}

	protected:
		template <typename type_t, typename = std::enable_if_t<std::is_assignable<type_t, arg_t>::value>>
		bool assign_assignable(const type_t& arg)
		{
			_value = arg;
			return true;
		}
		template <typename type_t, std::enable_if_t< ! std::is_assignable<type_t, arg_t>::value, int> = 0>
		bool assign_assignable(const type_t& arg)
		{
			return variant<args_t...>::assign_same(arg);
		}
	};

	template <typename arg_t>
	class variant<arg_t>
	{
		optional<arg_t> _value;

	public:
		variant() = default;

		template <typename type_t>
		variant(const type_t& arg)
		{
			assign(arg);
		}

		template <typename type_t>
		variant& operator=(const type_t& arg)
		{
			assign(arg);
			return *this;
		}

		arg_t value() const
		{
			return _value;
		}

		template <typename value_t, typename = std::enable_if_t<std::is_same<value_t, arg_t>::value>>
		value_t get() const
		{
			if (_value)
				return *_value;
			throw std::runtime_error("bad_variant_access");
		}

		template <typename callable_t>
		auto visit(const callable_t& cb) const
		{
			if (_value)
				return cb(*_value);
			throw std::runtime_error("bad_variant_access");
		}

	private:
		template <typename type_t>
		void assign(const type_t& arg)
		{
			if (assign_same(arg))
				return;
			if (assign_convertible(arg))
				return;
			throw std::runtime_error("bad_variant_assign");
		}

	protected:
		template <typename type_t, typename = std::enable_if_t<std::is_same<type_t, arg_t>::value>>
		bool assign_same(const type_t& arg)
		{
			_value = arg;
			return true;
		}
		template <typename type_t, std::enable_if_t< ! std::is_same<type_t, arg_t>::value, int> = 0>
		bool assign_same(const type_t& arg)
		{
			return false;
		}

	protected:
		template <typename type_t, typename = std::enable_if_t<std::is_convertible<type_t, arg_t>::value>>
		bool assign_convertible(const type_t& arg)
		{
			_value = static_cast<arg_t>(arg);
			return true;
		}
		template <typename type_t, std::enable_if_t< ! std::is_convertible<type_t, arg_t>::value, int> = 0>
		bool assign_convertible(const type_t& arg)
		{
			return false;
		}

	protected:
		template <typename type_t, typename = std::enable_if_t<std::is_assignable<type_t, arg_t>::value>>
		bool assign_assignable(const type_t& arg)
		{
			_value = arg;
			return true;
		}
		template <typename type_t, std::enable_if_t< ! std::is_assignable<type_t, arg_t>::value, int> = 0>
		bool assign_assignable(const type_t& arg)
		{
			return false;
		}
	};

	template <typename value_t, typename variant_t>
	auto get(const variant_t& v)
	{
		return v.template get<value_t>();
	}

	template <typename callable_t, typename variant_t>
	auto visit(const callable_t& cb, const variant_t& v)
	{
		return v.visit(cb);
	}
} //namespace utils
} //namespace charivari_ltd


