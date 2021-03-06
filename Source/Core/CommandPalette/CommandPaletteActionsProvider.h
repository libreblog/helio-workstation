/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

class CommandPaletteAction final : public ReferenceCountedObject
{
public:

    using Callback = Function<bool(TextEditor &ed)>;
    using Ptr = ReferenceCountedObjectPtr<CommandPaletteAction>;

    static CommandPaletteAction::Ptr action(String text, String hint, float order);
    CommandPaletteAction::Ptr withCallback(Callback callback);
    CommandPaletteAction::Ptr withColour(const Colour &colour);
    CommandPaletteAction::Ptr unfiltered();

    const String &getName() const noexcept;
    const String &getHint() const noexcept;
    const Colour &getColor() const noexcept;
    const Callback getCallback() const noexcept;
    const bool isUnfiltered() const noexcept;

    void setMatch(int score, const uint8 *matches);
    int getMatchScore() const noexcept;
    float getOrder() const noexcept;
    const GlyphArrangement &getGlyphArrangement() const noexcept;

private:

    CommandPaletteAction() = delete;
    CommandPaletteAction(String text, String hint, float order);

    String name;
    String hint;
    Colour colour = Colours::grey;
    Callback callback;

    int commandId = 0;
    bool shouldClosePalette = true;
    bool required = false;

    GlyphArrangement highlightedMatch;
    int matchScore = 0;

    // actions will be sorted by match, as user is entering the search text,
    // but we may also need ordering for the full list or items with the same match;
    // the context for this variable should be defined by action provider,
    // for example, timeline events will be ordered by position at the timeline
    // (see CommandPaletteActionSortByMatch)
    float order = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandPaletteAction)
};

class CommandPaletteActionsProvider
{
public:

    CommandPaletteActionsProvider() = default;
    virtual ~CommandPaletteActionsProvider() {};

    using Prefix = juce_wchar;
    virtual bool usesPrefix(const Prefix prefix) const = 0;

    using Actions = ReferenceCountedArray<CommandPaletteAction>;
    const Actions &getFilteredActions() const
    {
        return this->filteredActions;
    }

    virtual void updateFilter(const String &pattern, bool skipPrefix);
    virtual void clearFilter();

protected:

    virtual const Actions &getActions() const = 0;

private:

    Actions filteredActions;

    JUCE_DECLARE_WEAK_REFERENCEABLE(CommandPaletteActionsProvider)
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandPaletteActionsProvider)
};
