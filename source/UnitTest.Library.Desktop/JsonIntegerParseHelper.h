#pragma once
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include "Vector.h"

namespace UnitTests
{
	class JsonIntegerParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntegerParseHelper, FieaGameEngine::IJsonParseHelper)
	public:
		class SharedData final : public FieaGameEngine::JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FieaGameEngine::JsonParseCoordinator::SharedData)
		public:
			//virtual void Initialize() override;
			virtual gsl::owner<SharedData*> Create() const override;

			FieaGameEngine::Vector<std::int32_t> m_data;
		};

		virtual gsl::owner<FieaGameEngine::IJsonParseHelper*> Create() const override;
		virtual void Initialize() override;
        virtual bool StartHandler(FieaGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& jsonValue, bool isArray) override;
        virtual bool EndHandler(FieaGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key) override;

	private:
		inline static const std::string IntegerKey = "integer";
		bool m_parsingData = false;

	};
}

