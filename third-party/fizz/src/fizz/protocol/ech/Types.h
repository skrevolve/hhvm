/*
 *  Copyright (c) 2019-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <fizz/crypto/hpke/Types.h>
#include <fizz/record/Extensions.h>
#include <cstdint>
#include <vector>

namespace fizz {
namespace ech {

using Buf = std::unique_ptr<folly::IOBuf>;
using HpkePublicKey = Buf;

constexpr size_t kEchAcceptConfirmationSize = 8;

enum class ECHVersion : uint16_t {
  Draft15 = 0xfe0d,
};

struct ECHConfig {
  ECHConfig() = default;

  ECHConfig(const ECHConfig& other) {
    version = other.version;
    ech_config_content = other.ech_config_content->clone();
  }

  ECHConfig(ECHConfig&&) = default;

  ECHConfig& operator=(const ECHConfig& other) {
    if (this != &other) {
      version = other.version;
      ech_config_content = other.ech_config_content->clone();
    }
    return *this;
  }

  ECHConfig& operator=(ECHConfig&&) = default;

  ECHVersion version;
  Buf ech_config_content;
};

struct ECHConfigList {
  std::vector<ECHConfig> configs;
};

struct HpkeSymmetricCipherSuite {
  bool operator==(const HpkeSymmetricCipherSuite& other) const {
    return kdf_id == other.kdf_id && aead_id == other.aead_id;
  }
  bool operator!=(const HpkeSymmetricCipherSuite& other) const {
    return kdf_id != other.kdf_id || aead_id != other.aead_id;
  }

  hpke::KDFId kdf_id;
  hpke::AeadId aead_id;
};

struct HpkeKeyConfig {
  HpkeKeyConfig() = default;

  HpkeKeyConfig(const HpkeKeyConfig& other) {
    config_id = other.config_id;
    kem_id = other.kem_id;
    public_key = other.public_key->clone();
    cipher_suites = other.cipher_suites;
  }

  HpkeKeyConfig(HpkeKeyConfig&&) = default;

  HpkeKeyConfig& operator=(const HpkeKeyConfig& other) {
    if (this != &other) {
      config_id = other.config_id;
      kem_id = other.kem_id;
      public_key = other.public_key->clone();
      cipher_suites = other.cipher_suites;
    }
    return *this;
  }

  HpkeKeyConfig& operator=(HpkeKeyConfig&&) = default;

  uint8_t config_id{};
  hpke::KEMId kem_id{};
  HpkePublicKey public_key;
  std::vector<HpkeSymmetricCipherSuite> cipher_suites;
};

struct ECHConfigContentDraft {
  static folly::Optional<ECHConfigContentDraft> parseSupportedECHConfig(
      const ECHConfig&);

  ECHConfigContentDraft() = default;

  ECHConfigContentDraft(const ECHConfigContentDraft& other) {
    key_config = other.key_config;
    maximum_name_length = other.maximum_name_length;
    public_name = other.public_name;
    extensions = std::vector<Extension>();
    for (const auto& extension : other.extensions) {
      extensions.push_back(extension.clone());
    }
  }

  ECHConfigContentDraft(ECHConfigContentDraft&&) = default;

  ECHConfigContentDraft& operator=(const ECHConfigContentDraft& other) {
    if (this != &other) {
      key_config = other.key_config;
      maximum_name_length = other.maximum_name_length;
      public_name = other.public_name;
      extensions = std::vector<Extension>();
      for (const auto& extension : other.extensions) {
        extensions.push_back(extension.clone());
      }
    }
    return *this;
  }

  ECHConfigContentDraft& operator=(ECHConfigContentDraft&&) = default;

  HpkeKeyConfig key_config;
  uint8_t maximum_name_length{};
  std::string public_name;
  std::vector<Extension> extensions;
};

using ParsedECHConfig = ECHConfigContentDraft;

} // namespace ech
} // namespace fizz

#include <fizz/protocol/ech/Types-inl.h>
