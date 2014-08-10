
/*
*  AudioStream.hpp
*  sfeMovie project
*
*  Copyright (C) 2010-2014 Stephan Vedder
*  stephan.vedder@gmail.com
*
*  This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*
*/


#ifndef SFEMOVIE_SUBTITLESTREAM_HPP
#define SFEMOVIE_SUBTITLESTREAM_HPP

#include "Macros.hpp"
#include "Stream.hpp"
#include <SFML/Graphics.hpp>
#include <stdint.h>



namespace sfe
{
	class SubtitleStream : public Stream{
	public:
		struct Delegate {
			virtual void didUpdateSubtitle(const SubtitleStream& sender, const std::vector<sf::Sprite*>& subimages) = 0;
		};
		/** Create a subtitle stream from the given FFmpeg stream
		*
		* At the end of the constructor, the stream is guaranteed
		* to have all of its fields set and the decoder loaded
		*/
		SubtitleStream(AVFormatContext* formatCtx, AVStream* stream, DataSource& dataSource, Timer& timer, Delegate& delegate);

		/** Default destructor
		*/
		virtual ~SubtitleStream();

		/** Get the stream kind (either audio, video or subtitle stream)
		*
		* @return the kind of stream represented by this stream
		*/
		virtual MediaType getStreamKind() const;

		/** Update the stream's status
		*/
		virtual void update();
	private:
		struct SubImage
		{
			std::vector<sf::Sprite*> out;
			int64_t pts;	
			AVSubtitle sub;
		};

		std::vector<sf::Sprite*>  SubToSprites(AVSubtitle* sub);


		bool onGetData();

		Delegate& m_delegate;
		const uint32_t m_buffersize = 10;
		sf::Texture* m_texture;
		sf::Text m_subtext;
		std::vector<SubImage*> m_inactive;
		std::vector<SubImage*> m_active;
	};

};

#endif