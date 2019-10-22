//
// Created by expert on 14-05-19.
//

#ifndef LIBSTDAUDIO___ALSA_BACKEND_H
#define LIBSTDAUDIO___ALSA_BACKEND_H

#include <alsa/asoundlib.h>
#include <forward_list>

_LIBSTDAUDIO_NAMESPACE_BEGIN
struct audio_device_exception : runtime_error
{
	audio_device_exception( const char* description )
		: runtime_error( description )
	{
	}
};
class audio_device {
	public:
		audio_device() = delete;

		string_view name() const noexcept {
			return {};
		}

		using device_id_t = snd_pcm_t*;

		device_id_t device_id() const noexcept {
			return id;
		}

		bool is_input() const noexcept {
			return snd_pcm_stream( id ) == SND_PCM_STREAM_CAPTURE;
		}

		bool is_output() const noexcept {
			return snd_pcm_stream( id ) == SND_PCM_STREAM_PLAYBACK;
		}

		int get_num_input_channels() const noexcept {
			return 0;
		}

		int get_num_output_channels() const noexcept {
			return 0;
		}

		using sample_rate_t = unsigned;

		sample_rate_t get_sample_rate() const noexcept {
			return {};
		}

		span<const sample_rate_t> get_supported_sample_rates() const noexcept {
			return {};
		}

		bool set_sample_rate(sample_rate_t) {
			return false;
		}

		using buffer_size_t  = unsigned;

		buffer_size_t get_buffer_size_frames() const noexcept {
			return {};
		}

		span<const buffer_size_t> get_supported_buffer_sizes_frames() const noexcept {
			return {};
		}

		bool set_buffer_size_frames(buffer_size_t new_buffer_size) {
			return false;
		}

		template <typename _SampleType>
		constexpr bool supports_sample_type() const noexcept {
			return false;
		}

		constexpr bool can_connect() const noexcept {
			return false;
		}

		constexpr bool can_process() const noexcept {
			return false;
		}

		bool start() {
			return false;
		}

		bool stop() {
			return false;
		}

		bool is_running() const noexcept {
			return false;
		}

		void wait() const {
			assert(false);
		}

		template <typename _CallbackType>
		void process(_CallbackType&) {
			assert(false);
		}

		constexpr bool has_unprocessed_io() const noexcept {
			return false;
		}

	private:
		friend class audio_device_enumerator;
		enum class open_mode
		{
				nonblock = SND_PCM_NONBLOCK,
				async    = SND_PCM_ASYNC
		};
		audio_device ( string deviceName, snd_pcm_stream_t direction, open_mode mode )
		{
			assert( !deviceName.empty() );
			device_id_t res;
			auto openres = snd_pcm_open( &res, deviceName.data(), direction, static_cast< int >( mode ) );
			if ( openres == 0 )
			{
				throw audio_device_exception( "Could not open device");
			}
			else
			{
				id = res;
			}
		}
		device_id_t id = nullptr;
};

class audio_device_list: public forward_list< audio_device >
{
};

optional<audio_device> get_default_audio_input_device() {
	return {};
}

optional<audio_device> get_default_audio_output_device() {
	return {};
}

audio_device_list get_audio_input_device_list() {
	return {};
}

audio_device_list get_audio_output_device_list() {
	return {};
}

_LIBSTDAUDIO_NAMESPACE_END
#endif //LIBSTDAUDIO___ALSA_BACKEND_H
